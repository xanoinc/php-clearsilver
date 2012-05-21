--TEST--
Test hdf_write_string() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
hdf_read_file($hdf, dirname(__FILE__).'/test.hdf');
var_dump(hdf_write_string($hdf));
?>
--EXPECT--
string(1489) "arg1 = 1
var = 
Numbers {
hdf9 = 9
hdf14 = 14
}
Blah = wow
Foo = Worn Out
Foo {
Bar {
Baz {
0 = zero
0 {
num = #0
}
1 = one
2 = two
2 {
num = #2
}
3 = three
}
}
}
Wow {
Foo = 3
}
Outside {
0 {
Inside {
0 = 0
1 = 1
}
}
1 {
Inside {
2 = 2
3 = 3
}
}
2 : Outside.1
}
TestIf = 0
Days {
0 = 0
0 {
Abbr = Mon
}
1 = 1
1 {
Abbr = Tues
}
2 = 2
2 {
Abbr = Wed
}
3 = 3
3 {
Abbr = Thur
}
4 = 4
4 {
Abbr = Fri
}
5 = 5
5 {
Abbr = Sat
}
6 = 6
6 {
Abbr = Sun
}
}
Neg = -1
My {
Test : Days.0.Abbr
Test2 : Days.0
}
Color = #fffff
CGI {
box {
msgs {
0 {
ticket_id = 1
}
1 {
ticket_id = 2
}
2 {
ticket_id = 3
}
}
cur {
min_box_idx = 1
}
}
}
Query {
boxid = 2
split = 1
filter = 0
sort = t
sort_dir = u
topic = -1
}
Files {
0 {
Name = Desktop
Type = dir
Sub {
0 {
Name = Bookmarks.html
Type = file
}
1 {
Name = History.txt
Type = file
}
2 {
Name = Resume.doc
Type = file
}
3 {
Name = Favorites
Type = dir
Sub {
0 {
Name = foo
Type = file
}
1 {
Name = bar
Type = file
}
2 {
Name = boo
Type = file
}
3 {
Name = baz
Type = file
}
4 {
Name = faq
Type = file
}
5 {
Name = far
Type = file
}
}
}
}
}
1 {
Name = .cshrc
Type = file
}
2 {
Name = Mail
Type = dir
Sub {
0 {
Name = inbox
Type = file
}
1 {
Name = received
Type = file
}
2 {
Name = postponed
Type = file
}
}
}
}
EvarTests {
0 = <?cs alt:Foo ?>ERROR<?cs /alt ?>
1 = test1
2 = test 2 <?cs var:Blah ?>
}
CS_START = <?cs
CS_END = ?>
A = HELLO
B = WORLD
C = <?cs var:A ?>/<?cs var:B ?>
Biz {
Address2 = addr
}
parent_id = -1
faq {
topic_id = 1
sub_topic_id = -1
}
"