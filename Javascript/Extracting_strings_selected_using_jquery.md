# 마우스 드래그로 선택한 텍스트 가져오기 with JQuery

```javascript
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"> 
<head>
	<title>jQuery : GetSelectionText Demos</title> 

	<!-- jQuery --> 
    <script src="https://code.jquery.com/jquery-1.11.3.min.js" type="text/javascript"></script> 
    
    <script type="text/javascript">
    	$(document).ready(function(){
	    	$('p').mouseup(function(){
		   var txt = '';
		   if (window.getSelection) {
		    txt = window.getSelection();
       	       } else if (document.getSelection) {
		    txt = document.getSelection();
	       } else if (document.selection) {
		    txt = document.selection.createRange().text;
	       } else {
		    return;
	       }

		  txt = String(txt); // Type Casting	    
	
		  $('input').val(txt);
		});
	    });
    </script>
</head>
<body>
	<p>asbddefdfdvfvdfgf</p>
	<p>가나다라마바사아자!</p>
	<input type="text" />
</body>
</html>
```

## References

* http://egloos.zum.com/itbaby/v/4545272
