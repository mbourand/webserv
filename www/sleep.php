<!DOCTYPE html>
<html>
<head>
<title>2 seconds load time</title>
</head>
<body style="text-align: center;">
<?php
echo date('h:i:s') . "<br>";
sleep(2);
echo date('h:i:s');
?>
</body>
</html>