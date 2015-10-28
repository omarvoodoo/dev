
# HTML table에 스크롤바 넣기

Html code

```html
<div class="table-frame">
<table class="tftable" border="1">
<tr><th>Header 1</th><th>Header 2</th><th>Header 3</th><th>Header 4</th><th>Header 5</th></tr>
<tr><td>Row:1 Cell:1</td><td>Row:1 Cell:2</td><td>Row:1 Cell:3</td><td>Row:1 Cell:4</td><td>Row:1 Cell:5</td></tr>
<tr><td>Row:2 Cell:1</td><td>Row:2 Cell:2</td><td>Row:2 Cell:3</td><td>Row:2 Cell:4</td><td>Row:2 Cell:5</td></tr>
<tr><td>Row:3 Cell:1</td><td>Row:3 Cell:2</td><td>Row:3 Cell:3</td><td>Row:3 Cell:4</td><td>Row:3 Cell:5</td></tr>
<tr><td>Row:4 Cell:1</td><td>Row:4 Cell:2</td><td>Row:4 Cell:3</td><td>Row:4 Cell:4</td><td>Row:4 Cell:5</td></tr>
<tr><td>Row:5 Cell:1</td><td>Row:5 Cell:2</td><td>Row:5 Cell:3</td><td>Row:5 Cell:4</td><td>Row:5 Cell:5</td></tr>
<tr><td>Row:6 Cell:1</td><td>Row:6 Cell:2</td><td>Row:6 Cell:3</td><td>Row:6 Cell:4</td><td>Row:6 Cell:5</td></tr>
<tr><td>Row:7 Cell:1</td><td>Row:7 Cell:2</td><td>Row:7 Cell:3</td><td>Row:7 Cell:4</td><td>Row:7 Cell:5</td></tr>
<tr><td>Row:8 Cell:1</td><td>Row:8 Cell:2</td><td>Row:8 Cell:3</td><td>Row:8 Cell:4</td><td>Row:8 Cell:5</td></tr>
<tr><td>Row:9 Cell:1</td><td>Row:9 Cell:2</td><td>Row:9 Cell:3</td><td>Row:9 Cell:4</td><td>Row:9 Cell:5</td></tr>
<tr><td>Row:10 Cell:1</td><td>Row:10 Cell:2</td><td>Row:10 Cell:3</td><td>Row:10 Cell:4</td><td>Row:10 Cell:5</td></tr>
<tr><td>Row:11 Cell:1</td><td>Row:11 Cell:2</td><td>Row:11 Cell:3</td><td>Row:11 Cell:4</td><td>Row:11 Cell:5</td></tr>
<tr><td>Row:12 Cell:1</td><td>Row:12 Cell:2</td><td>Row:12 Cell:3</td><td>Row:12 Cell:4</td><td>Row:12 Cell:5</td></tr>
<tr><td>Row:13 Cell:1</td><td>Row:13 Cell:2</td><td>Row:13 Cell:3</td><td>Row:13 Cell:4</td><td>Row:13 Cell:5</td></tr>
<tr><td>Row:14 Cell:1</td><td>Row:14 Cell:2</td><td>Row:14 Cell:3</td><td>Row:14 Cell:4</td><td>Row:14 Cell:5</td></tr>
<tr><td>Row:15 Cell:1</td><td>Row:15 Cell:2</td><td>Row:15 Cell:3</td><td>Row:15 Cell:4</td><td>Row:15 Cell:5</td></tr>
<tr><td>Row:16 Cell:1</td><td>Row:16 Cell:2</td><td>Row:16 Cell:3</td><td>Row:16 Cell:4</td><td>Row:16 Cell:5</td></tr>
<tr><td>Row:17 Cell:1</td><td>Row:17 Cell:2</td><td>Row:17 Cell:3</td><td>Row:17 Cell:4</td><td>Row:17 Cell:5</td></tr>
<tr><td>Row:18 Cell:1</td><td>Row:18 Cell:2</td><td>Row:18 Cell:3</td><td>Row:18 Cell:4</td><td>Row:18 Cell:5</td></tr>
<tr><td>Row:19 Cell:1</td><td>Row:19 Cell:2</td><td>Row:19 Cell:3</td><td>Row:19 Cell:4</td><td>Row:19 Cell:5</td></tr>
<tr><td>Row:20 Cell:1</td><td>Row:20 Cell:2</td><td>Row:20 Cell:3</td><td>Row:20 Cell:4</td><td>Row:20 Cell:5</td></tr>
<tr><td>Row:21 Cell:1</td><td>Row:21 Cell:2</td><td>Row:21 Cell:3</td><td>Row:21 Cell:4</td><td>Row:21 Cell:5</td></tr>
<tr><td>Row:22 Cell:1</td><td>Row:22 Cell:2</td><td>Row:22 Cell:3</td><td>Row:22 Cell:4</td><td>Row:22 Cell:5</td></tr>
<tr><td>Row:23 Cell:1</td><td>Row:23 Cell:2</td><td>Row:23 Cell:3</td><td>Row:23 Cell:4</td><td>Row:23 Cell:5</td></tr>
<tr><td>Row:24 Cell:1</td><td>Row:24 Cell:2</td><td>Row:24 Cell:3</td><td>Row:24 Cell:4</td><td>Row:24 Cell:5</td></tr>
</table>
</div>
```

CSS code

```css
/* Table Styles */
.tftable {font-size:12px;color:#333333;width:100%;border-width: 1px;border-color: #729ea5;border-collapse: collapse;}
.tftable th {font-size:12px;background-color:#acc8cc;border-width: 1px;padding: 8px;border-style: solid;border-color: #729ea5;text-align:left;}
.tftable tr {background-color:#d4e3e5;}
.tftable td {font-size:12px;border-width: 1px;padding: 8px;border-style: solid;border-color: #729ea5;}
.tftable tr:hover {background-color:#ffffff;}

/* Holds table with scrollbar */
.table-frame {
    width: 100%;
    height: 400px;
    overflow: auto;
}
```

## References

- [Quora](https://www.quora.com/How-do-you-display-a-scroll-bar-onto-an-HTML-table)
- [jsfiddle.net](https://jsfiddle.net/mfj0oy7m/)
