--TEST--
Test hdf_obj_name() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
$node = hdf_get_node($hdf, 'data.node');
var_dump(hdf_obj_name($node));
?>
--EXPECTF--
string(4) "node"