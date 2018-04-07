<?php
if(isset($_POST['open']))
{
$file = fopen("smart.txt","w+") or die ("file not open");

$s = "open";
fputs($file,$s) or die ("Data not write");

fclose($file);
}
elseif(isset($_POST['close']))
{
$file = fopen("smart.txt","w+") or die ("file not open");

$s = "close";
fputs($file,$s) or die ("Data not write");

fclose($file);
}
elseif(isset($_POST['manual']))
{
$file = fopen("smart.txt","w+") or die ("file not open");

$s = "manual";
fputs($file,$s) or die ("Data not write");

fclose($file);
}
elseif(isset($_POST['automatic']))
{
$file = fopen("smart.txt","w+") or die ("file not open");

$s = "automatic";
fputs($file,$s) or die ("Data not write");

fclose($file);
}
elseif(isset($_POST['stop']))
{
$file = fopen("smart.txt","w+") or die ("file not open");

$s = "stop";
fputs($file,$s) or die ("Data not write");

fclose($file);
}
?>