--TEST--
Test hdf_obj_next() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
// recursively walk an HDF node, printing the contents
function tree_walk($node) 
{
  while ($node) {
    var_dump(hdf_obj_name($node), hdf_obj_value($node));
    tree_walk(hdf_obj_child($node));

    // get the next child node
    $node = hdf_obj_next($node);
  }
}

$hdf = hdf_init();

$arr = array('foo' => 'bar',
             1 => array('key' => 'value'));

// map the array to the HDF
hdf_set_value($hdf, '', $arr);

// iterate over the HDF
tree_walk($hdf);
?>
--EXPECT--
NULL
string(0) ""
string(3) "foo"
string(3) "bar"
string(1) "1"
string(0) ""
string(3) "key"
string(5) "value"