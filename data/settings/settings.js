var settings = settings || {};

(function() {
	settings.init = init;
	settings.showTab = showTab;
	settings.updateRangeValue = updateRangeValue;

	var throttleCanvas;
	var loadedTabs = 0;

	function init() {
		$('#controls').load('controls.html', postInit);
		$('#motor').load('motor.html', postInit);
		$('#charger').load('charger.html', postInit);
		$('#dcdc').load('dcdc.html', postInit);
		$('#inputs').load('inputs.html', postInit);
		$('#outputs').load('outputs.html', postInit);
		$('#devices').load('devices.html', postInit);
		$('#about').load('about.html', postInit);
		window.addEventListener('resize', resizeThrottleCanvas, false);
	}

	function postInit() {
		if (++loadedTabs < 8) {
			return;
		}
		generateRangeControls();
		showTab('controls');
	}

	function showTab(pageId) {
		$('.tab').hide();
		$('#' + pageId).show();

		if (pageId == 'controls') {
			resizeThrottleCanvas();
		}
		loadData(pageId);
	}

	// load data from dynamic json and replace values in input fields, div's, gauges
	function loadData(pageId) {
		$.getJSON("/config", function(data) {
			showDevices(data.devices);
			for (name in data[pageId]) {
				setNodeValue(name, data[pageId][name]);
			}
			if (pageId == 'controls') {
				refreshThrottleVisualization();
			}
		});
	}

	function resizeThrottleCanvas() {
		// adjust the width to the page width
		var canvasElement = document.getElementById("throttleCanvas");
		if (canvasElement) {
			canvasElement.width = window.innerWidth - 30;
		}
		refreshThrottleVisualization();
	}

	function updateRangeValue(id, source) {
		var val = parseInt(source.value);

		if (val < 0 || isNaN(val))
			val = 0;

		if (id == 'minimumLevel') {
			var max = getIntValue("maximumLevel");
			if (val > max)
				val = max;
		} else if (id == 'maximumLevel') {
			var min = getIntValue("minimumLevel");
			if (val < min)
				val = min;
		} else if (id == 'minimumLevel2') {
			var max = getIntValue("maximumLevel2");
			if (val > max)
				val = max;
		} else if (id == 'maximumLevel2') {
			var min = getIntValue("minimumLevel2");
			if (val < min)
				val = min;
		} else if (id == 'positionRegenMaximum') {
			var regen = getIntValue("positionRegenMinimum");
			if (val > regen)
				val = regen;
		} else if (id == 'positionRegenMinimum') {
			var regen = getIntValue("positionRegenMaximum");
			var fwd = getIntValue("positionForwardStart");
			if (val < regen)
				val = regen;
			if (val > fwd)
				val = fwd;
		} else if (id == 'positionForwardStart') {
			var regen = getIntValue("positionRegenMinimum");
			var half = getIntValue("positionHalfPower");
			if (val < regen)
				val = regen;
			if (val > half)
				val = half;
		} else if (id == 'positionHalfPower') {
			var half = getIntValue("positionForwardStart");
			if (val < half)
				val = half;
		} else if (id == 'minimumRegen') {
			var regen = getIntValue("maximumRegen");
			if (val > regen)
				val = regen;
		} else if (id == 'maximumRegen') {
			var regen = getIntValue("minimumRegen");
			if (val < regen)
				val = regen;
		} else if (id == 'brakeMinimumLevel') {
			var max = getIntValue("brakeMaximumLevel");
			if (val > max)
				val = max;
		} else if (id == 'brakeMaximumLevel') {
			var min = getIntValue("brakeMinimumLevel");
			if (val < min)
				val = min;
		} else if (id == 'brakeMinimumRegen') {
			var max = getIntValue("brakeMaximumRegen");
			if (val > max)
				val = max;
		} else if (id == 'brakeMaximumRegen') {
			var min = getIntValue("brakeMinimumRegen");
			if (val < min)
				val = min;
		}

		$('#' + id).val(val);
		source.value = val;
		refreshThrottleVisualization();
	}

	function refreshThrottleVisualization() {
		if (!throttleCanvas) {
			throttleCanvas = new ThrottleSettingsCanvas();
		}
		throttleCanvas.draw();
	}

	function getIntValue(id) {
		var value = $('#' + id).val();
		return value ? parseInt(value) : 0;
	}

	function generateRangeControls() {
		addRangeControl("minimumLevel", 0, 4095);
		addRangeControl("minimumLevel2", 0, 4095);
		addRangeControl("maximumLevel", 0, 4095);
		addRangeControl("maximumLevel2", 0, 4095);
		addRangeControl("positionRegenMaximum", 0, 100, "%");
		addRangeControl("positionRegenMinimum", 0, 100, "%");
		addRangeControl("positionForwardStart", 0, 100, "%");
		addRangeControl("positionHalfPower", 0, 100, "%");
		addRangeControl("minimumRegen", 0, 100, "%");
		addRangeControl("maximumRegen", 0, 100, "%");
		addRangeControl("creepLevel", 0, 30, "%");
		addRangeControl("creepSpeed", 0, 2000, "rpm");
		addRangeControl("slewRate", 0, 1000, "%/sec");
		addRangeControl("brakeHold", 0, 50, "%");
		addRangeControl("brakeMinimumLevel", 0, 4095);
		addRangeControl("brakeMinimumRegen", 0, 100, "%");
		addRangeControl("brakeMaximumLevel", 0, 4095);
		addRangeControl("brakeMaximumRegen", 0, 100, "%");
		addRangeControl("inputCurrent", 1, 50);
	}

	function addRangeControl(id, min, max, unit) {
		var node = $('#' + id + 'Span');
		var title = node.attr('title');
		var dataDevice = node.attr('data-device');
		
		node.replaceWith('<input ' + (dataDevice ? 'data-device="' + dataDevice + '" ' : '') + 'id="' + id + 'Level" type="range" min="' + min
				+ '" max="' + max + '" onchange="settings.updateRangeValue(\''
				+ id + '\', this);" oninput="settings.updateRangeValue(\'' + id
				+ '\', this);" />'
				+ '<div ' + (dataDevice ? 'data-device="' + dataDevice + '" ' : '') + 'class="value"><input type="number" id="' + id
				+ '" name="' + id + '" min="' + min + '" max="' + max
				+ '" maxlength="4" size="4"'
				+ ' onchange="settings.updateRangeValue(\'' + id
				+ 'Level\', this);"/>' + (unit ? ' ' + unit : '') + '</div>');
	}

	// shows elements with a data-device attribute that contains the given device id and is activated.
	function showDevices(config) {
		$('[data-device]').filter(function() {
			var devices = $(this).attr('data-device').split(',');
			var activeDevices = devices.filter(device => config[device] !== 0);
			$(this).toggle(activeDevices.length > 0);
		});
	}
})();
