# CSS, vertical-align을 이용한 세로 정렬

table, table-cell, vertical-align을 이용하여 세로 정렬을 하려면 정렬을 하려는 `블록의 높이가 고정이어야 한다`.

## Example

#### HTML
```html
<section class='content-container'>
    <div class='statistics-layer'>
        <div class="statistics">
            <div class="count">
                <div>
                    <h1>13명 / 35명</h1>
                </div>
            </div>
            <div class="address">
                <div>
                    <h4>성동구 성수동 1가 656-1212</h4>
                </div>
            </div>
            <div class="time">
                <div>
                    02/15
                    <h1>11:48분</h1>
                </div>
            </div>
        </div>
    </div>
    <article>
    </article>
</section>
```

#### CSS
```css
.content-container .statistics {
    display: flex;
    text-align: center;
}

.content-container .statistics .count {
    width: 25%;
    height: 100px;
    display: table;
}

.content-container .statistics .count div {
    display: table-cell;
    vertical-align: middle;
}

.content-container .statistics .address {
    width: 50%;
    height: 100px;
    display: table;
}

.content-container .statistics .address div {
    display: table-cell;
    vertical-align: middle;
}

.content-container .statistics .time {
    width: 25%;
    height: 100px;
    display: table;
}

.content-container .statistics .time div {
    display: table-cell;
    vertical-align: middle;
}
```
