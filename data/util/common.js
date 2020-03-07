var nodecache = new Array();

function getCache(name) {
	var target = nodecache[name];
	if (!target) {
		target = document.getElementById(name);
		if (!target) { // id not found, try to find by name
			var namedElements = document.getElementsByName(name);
			if (namedElements && namedElements.length) {
				target = namedElements[0];
			}
		}
		nodecache[name]=target;
	}
	return target;
}

function setNodeValue(name, value) {
	// a normal div/span/input to update
	target = getCache(name);
	if (target) { // found an element, update according to its type
		if (target.nodeName.toUpperCase() == 'DIV' || target.nodeName.toUpperCase() == 'SPAN') {
			target.innerHTML = value;
		} else if (target.nodeName.toUpperCase() == 'INPUT') {
			if (target.type && (target.type.toUpperCase() == 'CHECKBOX' || target.type.toUpperCase() == 'RADIO')) {
				target.checked = value;
			} else {
				target.value = value;
				// find corresponding slider element
				var slider = document.getElementById(name + "Level");
				if (slider) {
					slider.value = value;
				}
			}
		} else if (target.nodeName.toUpperCase() == 'SELECT') {
			selectItemByValue(target, value);
		}
	}
}


// scan through the options of a select input field and activate the one with
// the given value
function selectItemByValue(node, value) {
	for (var i = 0; i < node.options.length; i++) {
		if (node.options[i].value == value) {
			node.selectedIndex = i;
			break;
		}
	}
}
