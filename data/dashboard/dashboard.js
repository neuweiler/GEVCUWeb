var dashboard = dashboard || {};

(function() {
	dashboard.init = init;
	dashboard.sendMsg = sendMsg;

	var webSocketWorker;

	function init(data) {
		updateSystemState(0);
		$('#cruiseControl').hide();
	
		alertify.set('notifier', 'position', 'bottom-right');

		activate();

		$.getJSON("/config", function(data) {
			setTimeout(function() { // delay generation of gauges to allow loading of Led font
				gauges.createDc(data.dashboard);
				gauges.createMotor(data.dashboard);
				gauges.createTemperature(data.dashboard);
			}, 250);
			setCruiseData(data.dashboard);
			setChargeLevels(data.dashboard);
		});
	}

	function activate() {
		// add an event listener so sounds can get loaded on mobile devices after user interaction
		window.addEventListener('keydown', removeBehaviorsRestrictions);
		window.addEventListener('mousedown', removeBehaviorsRestrictions);
		window.addEventListener('touchstart', removeBehaviorsRestrictions);
		startWebSocketWorker();
	}

	function removeBehaviorsRestrictions() {
		soundError.load();
		soundWarn.load();
		soundInfo.load();
		window.removeEventListener('keydown', removeBehaviorsRestrictions);
		window.removeEventListener('mousedown', removeBehaviorsRestrictions);
		window.removeEventListener('touchstart', removeBehaviorsRestrictions);
	}

	function startWebSocketWorker() {
		if (!webSocketWorker) {
			webSocketWorker = new Worker("webSocketWorker.js");
			webSocketWorker.onmessage = function(event) {
				processWebsocketMessage(event.data)
			};
		}
		webSocketWorker.postMessage({cmd : 'start'});
	}

	function stopWebSocketWorker() {
		if (webSocketWorker) {
			webSocketWorker.postMessage({cmd : 'stop'});
			webSocketWorker = null;
		}
	}

	function processWebsocketMessage(message) {
		for (name in message) {
			var data = message[name];
			var dial = Gauge.dials[name];
			if (dial) {
				dial.setValue(data);
			} else if (name == 'limits') {
				for (limitName in data) {
					var limit = data[limitName];
					dial = Gauge.dials[limitName];
					if (dial) {
						dial.setLimits(limit.min, limit.max);
					}
				}
			} else if (name.indexOf('bitfield') != -1) {
				annunciator.update(name, data);
			} else if (name == 'systemState') {
				updateSystemState(data);
			} else if (name == 'logMessage') {
				logMessage(data.level, data.message);
			} else if (name == 'enableCruiseControl') {
				showHideCruiseControl(data);
			} else {
				$('#' + name + 'Meter').attr('value', data); // set the meter value (additionally to a text node)
				setNodeValue(name, data);
			}
		}
	}

	function showHideCruiseControl(enabled) {
		if (enabled) {
			$('#cruiseControl').slideDown();
		} else {
			$('#cruiseControl').slideUp();
		}
	}

	function logMessage(level, message) {
		if (level == 'ERROR') {
			alertify.error(message, 0);
			soundError.play();
		} else if (level == 'WARNING') {
			alertify.warning(message, 60);
			soundWarn.play();
		} else {
			alertify.success(message, 30);
			soundInfo.play();
		}
	}

	// hide/show elements depending on system state
	function updateSystemState(state) {
		 $("[data-states]").filter(function() {
		      $(this).toggle($(this).attr('data-states').indexOf('_' + state + '_') > -1)
		    });
	}

	// send message to GEVCU via websocket
	function sendMsg(message) {
		console.log('sendMsg: ' + message);
		webSocketWorker.postMessage({cmd: 'message', message: message});
	}
	
	function setChargeLevels(data) {
		$('#maximumInputCurrentLevel').attr('min', data.chargeInputLevels[0]);
		$('#maximumInputCurrentLevel').attr('max', data.chargeInputLevels[data.chargeInputLevels.length - 1]);
	}
	
	function setCruiseData(data) {
		$('cruiseSpeedUnit').html(data.cruiseUseRpm ? 'rpm' : 'kmh');
		addCruiseButton('+' + data.cruiseSpeedStep, data.cruiseUseRpm);
		addCruiseButton('-' + data.cruiseSpeedStep, data.cruiseUseRpm);
		for (var speed in data.cruiseSpeedSet) {
			addCruiseButton(data.cruiseSpeedSet[speed], data.cruiseUseRpm);
		}
	}

	function addCruiseButton(speed, unit) {
		$('<button class="button" onclick="dashboard.sendMsg(\'cruise=' + speed +'\')">' + speed + '</button>')
			.appendTo('#cruiseControl');
	}
})();
