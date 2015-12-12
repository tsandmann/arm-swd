R""//(<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" media="all" href="/style.css" />
    <script src="/polyfill.js" type="text/javascript"></script>
    <script src="/script.js" type="text/javascript"></script>

	<title>swd:io</title>
</head>
<body>

<p>
	[<a href="/">home</a>]
</p>

<swd-begin>

	<p>
		To do: common Cortex-M, ARM, and other registers here
	</p>

	<swd-hexedit addr="0x40000000" count="32" ></swd-hexedit>
	<swd-hexedit addr="0x40010000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x40020000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x40030000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x40040000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x40050000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x40060000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x40070000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x40080000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x40090000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x400a0000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x400b0000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x400c0000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x400d0000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x400e0000" count="4"  ></swd-hexedit>
	<swd-hexedit addr="0x400f0000" count="16" ></swd-hexedit>
	<swd-hexedit addr="0x400ff000" count="16" ></swd-hexedit>

</swd-begin>

</body>
</html>
)"//";