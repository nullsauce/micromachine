
function char_pad(n, width, pad) {
	n = n + '';
	return n.length >= width ? n : new Array(width - n.length + 1).join(pad) + n;
}

function zpad(n, width) {
	return char_pad(n, width, '0');
}

function hex32(n) {
	return zpad(n.toString(16), 8)
}
