package main

import (
	"database/sql"
	_ "github.com/go-sql-driver/mysql"
	"gopkg.in/gorp.v1"
	"log"
	"time"
	"fmt"
)

const (
	dbname = "userdb"
	dbuser = "username"
	dbpassword = "userpass"
	dbip = "localhost"
	dbport = "3306"
)

type Person struct {
    Id      int64
    Created int64
    Updated int64
    FName   string
    LName   string
}

func main() {
	dbmap := initDb()
	defer dbmap.Db.Close()

	var err error

	// delete any existing rows
	err = dbmap.TruncateTables()
	checkErr(err, "TruncateTables failed")

	p1 := Person {
		Created: time.Now().UnixNano(),
		Updated: time.Now().UnixNano(),
		FName: "namename",
		LName: "lnamelnam",
	}

	p2 := Person {
		Created: time.Now().UnixNano(),
		Updated: time.Now().UnixNano(),
		FName: "2namename",
		LName: "2lnamelnam",
	}

	log.Println("before, p1.Id:", p1.Id)
	log.Println("before, p2.Id:", p2.Id)

	err = dbmap.Insert(&p1, &p2)
	checkErr(err, "Insert failed")

	log.Println("after, p1.Id:", p1.Id)
	log.Println("after, p2.Id:", p2.Id)

	// use convenience SelectInt
	count, err := dbmap.SelectInt("select count(*) from Person")
	checkErr(err, "select count(*) failed")
	log.Println("Rows after inserting:", count)

	// update a row
	p2.FName = "2-2namename"
	count, err = dbmap.Update(&p2)
	checkErr(err, "Update failed")
	log.Println("Rows updated:", count)

	// fetch one row
	err = dbmap.SelectOne(&p2, "select * from Person where Id=?", p2.Id)
	checkErr(err, "SelectOne failed")
	log.Println("p2 row:", p2)

	// fetch all rows
	var persons []Person
	_, err = dbmap.Select(&persons, "select * from Person order by Id")
	checkErr(err, "Select failed")
	log.Println("All rows:")
	for x, p := range persons {
		log.Printf("	%d: %v\n", x, p)
	}

	// delete row by PK
	count, err = dbmap.Delete(&p1)
	checkErr(err, "Delete failed")
	log.Println("Rows deleted:", count)

	// delete row manually via Exec
	_, err = dbmap.Exec("delete from Person where Id=?", p2.Id)
	checkErr(err, "Exec failed")

	// confirm count is zero
	count, err = dbmap.SelectInt("select count(*) from Person")
	checkErr(err, "select count(*) failed")
	log.Println("Row count - should be zero:", count)

	log.Println("Done!")
}

func getDataSourceName() string {
	var dataSourceName = fmt.Sprintf( 
		"%s:%s@tcp(%s:%s)/%s",
		dbuser, dbpassword, dbip, dbport, dbname)
	return dataSourceName
}

func initDb() *gorp.DbMap {
	var dataSourceName = getDataSourceName()
	db, err := sql.Open("mysql", dataSourceName)
	checkErr(err, "sql.Open failed")

	dbmap := &gorp.DbMap{Db: db, Dialect: gorp.MySQLDialect{"InnoDB", "UTF8"}}
	dbmap.AddTableWithName(Person{}, "Person").SetKeys(true, "Id")

	err = dbmap.CreateTablesIfNotExists()
	checkErr(err, "Create tables failed")

	return dbmap
}

func checkErr(err error, msg string) {
	if err != nil {
		log.Fatalln(msg, err)
	}
}
