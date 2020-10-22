var dashboard = dashboard || {};

(function() {
	dashboard.init = init;
	dashboard.sendMsg = sendMsg;

	var webSocketWorker;
	var systemStateInterval;

	function init(data) {
		updateSystemState(0);
		$('#cruiseControl').hide();
	
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
		scheduleSystemStateCheck();
	}

	function removeBehaviorsRestrictions() {
		soundError.load();
		soundWarn.load();
		soundInfo.load();
		window.removeEventListener('keydown', removeBehaviorsRestrictions);
		window.removeEventListener('mousedown', removeBehaviorsRestrictions);
		window.removeEventListener('touchstart', removeBehaviorsRestrictions);
	}

	// a workaround for failed connects/system-state transmission
	function scheduleSystemStateCheck() {
		systemStateInterval = setInterval(function () {
			sendMsg('connected');
		}, 5000);
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
			} else if (name.startsWith('limits')) {
				var limitTokens = name.split('.');
				dial = Gauge.dials[limitTokens[1]];
				if (dial) {
					if (limitTokens[2] === 'min') {
						dial.setLimitsMin(data);
					} else if (limitTokens[2] === 'max') {
						dial.setLimitsMax(data);
					}
				}
			} else if (name.indexOf('bitfield') != -1) {
				annunciator.update(name, data);
			} else if (name === 'systemState') {
				updateSystemState(data);
			} else if (name === 'logMessage') {
				logMessage(data.level, data.message);
			} else if (name === 'enableCruiseControl') {
				showHideCruiseControl(data);
			} else {
				$('#' + name + 'Meter').attr('value', data); // set the meter value (additionally to a text node)
				setNodeValue(name, data);
				if (name === 'cruiseControlSpeed') {
					showHideCruiseSpeed(data);
				}
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

	function showHideCruiseSpeed(speed) {
		if (speed && speed > 0) {
			$('#cruiseInfo').slideDown();
		} else {
			$('#cruiseInfo').slideUp();
		}
	}

	function logMessage(level, message) {
		if (level === 'ERROR') {
			VanillaToasts.create({
				text: message,
				type: 'error',
				icon: '/pics/logError.png',
				positionClass: 'bottomCenter',
				timeout: 0
			});
			soundError.play();
		} else if (level === 'WARNING') {
			VanillaToasts.create({
				text: message,
				type: 'warning',
				icon: '/pics/logWarn.png',
				positionClass: 'bottomCenter',
				timeout: 30000
			});
			soundWarn.play();
		} else {
			VanillaToasts.create({
				text: message,
				type: 'info',
				icon: '/pics/logInfo.png',
				positionClass: 'bottomCenter',
				timeout: 15000
			});
			soundInfo.play();
		}
	}

	// hide/show elements depending on system state
	function updateSystemState(state) {
		if (systemStateInterval) {
			clearInterval(systemStateInterval);
			systemStateInterval = null;
		}
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
		$('#maximumInputCurrentLevel').attr('max', data.chargeInputLevels[1]);
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
