--TEST--
Test cs_render() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
hdf_set_value($hdf, 'message', 'Hello World!');
$cs = cs_init($hdf);
$template = 'Your message is "<?cs var message ?>".';
cs_parse_string($cs, $template);
var_dump(cs_render($cs));
?>
--EXPECT--
string(31) "Your message is "Hello World!"."