R""//(<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" media="all" href="/style.css" />
    <script src="/polyfill.js" type="text/javascript"></script>
    <script src="/script.js" type="text/javascript"></script>

	<title>swd:mem</title>
</head>
<body>

<p>
	[<a href="/">home</a>]
</p>

<swd-begin>

	<p>
		<a is="swd-hexnav" href="#" nav-step="-0x10000">&lt;--64k</a>
		<a is="swd-hexnav" href="#" nav-step="-0x1000">&lt;&lt;4k</a>
		<a is="swd-hexnav" href="#" nav-step="-0x100">&lt;256</a>
		---
		<a is="swd-hexnav" href="#" nav-step="0x100">256&gt;</a>
		<a is="swd-hexnav" href="#" nav-step="0x1000">4k&gt;&gt;</a>
		<a is="swd-hexnav" href="#" nav-step="0x10000">64k--&gt;</a>
	</p>

	<swd-hexedit navigation="true"></swd-hexedit>

	<p>
		<a is="swd-hexnav" href="#" nav-step="-0x10000">&lt;--64k</a>
		<a is="swd-hexnav" href="#" nav-step="-0x1000">&lt;&lt;4k</a>
		<a is="swd-hexnav" href="#" nav-step="-0x100">&lt;256</a>
		---
		<a is="swd-hexnav" href="#" nav-step="0x100">256&gt;</a>
		<a is="swd-hexnav" href="#" nav-step="0x1000">4k&gt;&gt;</a>
		<a is="swd-hexnav" href="#" nav-step="0x10000">64k--&gt;</a>
	</p>

</swd-begin>

</body>
</html>
)"//";
