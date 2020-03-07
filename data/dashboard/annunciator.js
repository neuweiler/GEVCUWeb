var annunciator = annunciator || {};

(function() {
	annunciator.update = update;
	annunciator.fold = fold;

	var FieldClass = {
		off		: "annunciatorOff",
		warn	: "annunciatorWarn",
		error	: "annunciatorError",
		ok		: "annunciatorOk"
	};

	/* These BitField definitions must be kept in synch with GEVCU's Status.cpp */
	var Motor = {
		warning                            : 1 << 0,
		oscillationLimiter                 : 1 << 1,
		maximumModulationLimiter           : 1 << 2,

		overtempController                 : 1 << 8,
		overtempMotor                      : 1 << 9,
		overspeed                          : 1 << 10,
		hvUndervoltage                     : 1 << 11,
		hvOvervoltage                      : 1 << 12,
		hvOvercurrent                      : 1 << 13,
		acOvercurrent                      : 1 << 14,

		limitationTorque                   : 1 << 16,
		limitationMaxTorque                : 1 << 17,
		limitationSpeed                    : 1 << 18,
		limitationControllerTemperature    : 1 << 19,
		limitationMotorTemperature         : 1 << 20,
		limitationSlewRate                 : 1 << 21,
		limitationMotorModel               : 1 << 22,
		limitationMechanicalPower          : 1 << 23,
		limitationAcVoltage                : 1 << 24,
		limitationAcCurrent                : 1 << 25,
		limitationDcVoltage                : 1 << 26,
		limitationDcCurrent                : 1 << 27
	};
	
	var BMS = {
		bmsRelayDischarge                  : 1 << 0,
		bmsRelayCharge                     : 1 << 1,
		bmsChagerSafety                    : 1 << 2,
		bmsDtcPresent                      : 1 << 3,

		bmsVoltageFailsafe                 : 1 << 8,
		bmsCurrentFailsafe                 : 1 << 9,
		bmsDepleted                        : 1 << 10,
		bmsBalancingActive                 : 1 << 11,
		bmsDtcWeakCellFault                : 1 << 12,
		bmsDtcLowCellVolage                : 1 << 13,
		bmsDtcHVIsolationFault             : 1 << 14,
		bmsDtcVoltageRedundancyFault       : 1 << 15,

		bmsDclLowSoc                       : 1 << 16,
		bmsDclHighCellResistance           : 1 << 17,
		bmsDclTemperature                  : 1 << 18,
		bmsDclLowCellVoltage               : 1 << 19,
		bmsDclLowPackVoltage               : 1 << 20,
		bmsDclCclVoltageFailsafe           : 1 << 21,
		bmsDclCclCommunication             : 1 << 22,
		bmsCclHighSoc                      : 1 << 23,
		bmsCclHighCellResistance           : 1 << 24,
		bmsCclTemperature                  : 1 << 25,
		bmsCclHighCellVoltage              : 1 << 26,
		bmsCclHighPackVoltage              : 1 << 27,
		bmsCclChargerLatch                 : 1 << 28,
		bmsCclAlternate                    : 1 << 29
	};

	var IO = {
		brakeHold                          : 1 << 0,
		preChargeRelay                     : 1 << 1,
		secondaryContactor                 : 1 << 2,
		mainContactor                      : 1 << 3,
		enableMotor                        : 1 << 4,
		coolingFan                         : 1 << 5,
		brakeLight                         : 1 << 6,
		reverseLight                       : 1 << 7,
		enableIn                           : 1 << 8,
		absActive                          : 1 << 9
	};

	var foldTimeout = null;
	var openedAutomatically = false;
	var warningMotor = false;
	var warningBms = false;

	function update(name, bitfield) {
		switch (name) {
		case 'bitfieldMotor':
			updateField("warning"                       , FieldClass.warn, bitfield & Motor.warning);
			updateField("oscillationLimiter"            , FieldClass.ok, bitfield & Motor.oscillationLimiter);
			updateField("maximumModulationLimiter"      , FieldClass.warn, bitfield & Motor.maximumModulationLimiter);

			updateField("overtempController"            , FieldClass.error, bitfield & Motor.overtempController);
			updateField("overtempMotor"                 , FieldClass.error, bitfield & Motor.overtempMotor);
			updateField("overspeed"                     , FieldClass.error, bitfield & Motor.overspeed);
			updateField("hvUndervoltage"                , FieldClass.error, bitfield & Motor.hvUndervoltage);
			updateField("hvOvervoltage"                 , FieldClass.error, bitfield & Motor.hvOvervoltage);
			updateField("hvOvercurrent"                 , FieldClass.error, bitfield & Motor.hvOvercurrent);
			updateField("acOvercurrent"                 , FieldClass.error, bitfield & Motor.acOvercurrent);

			updateField("limitationTorque"              , FieldClass.warn, bitfield & Motor.limitationTorque);
			updateField("limitationMaxTorque"           , FieldClass.warn, bitfield & Motor.limitationMaxTorque);
			updateField("limitationSpeed"               , FieldClass.warn, bitfield & Motor.limitationSpeed);
			updateField("limitationControllerTemperature", FieldClass.warn, bitfield & Motor.limitationControllerTemperature);
			updateField("limitationMotorTemperature"    , FieldClass.warn, bitfield & Motor.limitationMotorTemperature);
			updateField("limitationSlewRate"            , FieldClass.warn, bitfield & Motor.limitationSlewRate);
			updateField("limitationMotorModel"          , FieldClass.warn, bitfield & Motor.limitationMotorModel);
			updateField("limitationMechanicalPower"     , FieldClass.warn, bitfield & Motor.limitationMechanicalPower);
			updateField("limitationAcVoltage"           , FieldClass.warn, bitfield & Motor.limitationAcVoltage);
			updateField("limitationAcCurrent"           , FieldClass.warn, bitfield & Motor.limitationAcCurrent);
			updateField("limitationDcVoltage"           , FieldClass.warn, bitfield & Motor.limitationDcVoltage);
			updateField("limitationDcCurrent"           , FieldClass.warn, bitfield & Motor.limitationDcCurrent);

			warningMotor = (bitfield & ~~(Motor.oscillationLimiter | Motor.limitationTorque | Motor.limitationSpeed |
					Motor.limitationMaxTorque | Motor.limitationSlewRate | Motor.limitationMotorModel |
					Motor.limitationMechanicalPower | Motor.limitationAcCurrent | Motor.limitationDcCurrent)) !== 0;
			break;

		case 'bitfieldBms':
			updateField("bmsRelayDischarge"             , FieldClass.ok, bitfield & BMS.bmsRelayDischarge);
			updateField("bmsRelayCharge"                , FieldClass.ok, bitfield & BMS.bmsRelayCharge);
			updateField("bmsChagerSafety"               , FieldClass.ok, bitfield & BMS.bmsChagerSafety);
			updateField("bmsDtcPresent"                 , FieldClass.error, bitfield & BMS.bmsDtcPresent);

			updateField("bmsVoltageFailsafe"            , FieldClass.error, bitfield & BMS.bmsVoltageFailsafe);
			updateField("bmsCurrentFailsafe"            , FieldClass.error, bitfield & BMS.bmsCurrentFailsafe);
			updateField("bmsDepleted"                   , FieldClass.error, bitfield & BMS.bmsDepleted);
			updateField("bmsBalancingActive"            , FieldClass.ok, bitfield & BMS.bmsBalancingActive);
			updateField("bmsDtcWeakCellFault"           , FieldClass.error, bitfield & BMS.bmsDtcWeakCellFault);
			updateField("bmsDtcLowCellVolage"           , FieldClass.error, bitfield & BMS.bmsDtcLowCellVolage);
			updateField("bmsDtcHVIsolationFault"        , FieldClass.error, bitfield & BMS.bmsDtcHVIsolationFault);
			updateField("bmsDtcVoltageRedundancyFault"  , FieldClass.error, bitfield & BMS.bmsDtcVoltageRedundancyFault);

			updateField("bmsDclLowSoc"                  , FieldClass.warn, bitfield & BMS.bmsDclLowSoc);
			updateField("bmsDclHighCellResistance"      , FieldClass.warn, bitfield & BMS.bmsDclHighCellResistance);
			updateField("bmsDclTemperature"             , FieldClass.warn, bitfield & BMS.bmsDclTemperature);
			updateField("bmsDclLowCellVoltage"          , FieldClass.warn, bitfield & BMS.bmsDclLowCellVoltage);
			updateField("bmsDclLowPackVoltage"          , FieldClass.warn, bitfield & BMS.bmsDclLowPackVoltage);
			updateField("bmsDclCclVoltageFailsafe"      , FieldClass.warn, bitfield & BMS.bmsDclCclVoltageFailsafe);
			updateField("bmsDclCclCommunication"        , FieldClass.warn, bitfield & BMS.bmsDclCclCommunication);
			updateField("bmsCclHighSoc"                 , FieldClass.warn, bitfield & BMS.bmsCclHighSoc);
			updateField("bmsCclHighCellResistance"      , FieldClass.warn, bitfield & BMS.bmsCclHighCellResistance);
			updateField("bmsCclTemperature"             , FieldClass.warn, bitfield & BMS.bmsCclTemperature);
			updateField("bmsCclHighCellVoltage"         , FieldClass.warn, bitfield & BMS.bmsCclHighCellVoltage);
			updateField("bmsCclHighPackVoltage"         , FieldClass.warn, bitfield & BMS.bmsCclHighPackVoltage);
			updateField("bmsCclChargerLatch"            , FieldClass.warn, bitfield & BMS.bmsCclChargerLatch);
			updateField("bmsCclAlternate"               , FieldClass.warn, bitfield & BMS.bmsCclAlternate);

			warningBms = (bitfield & ~~(BMS.bmsRelayDischarge | BMS.bmsRelayCharge | BMS.bmsChagerSafety |
					BMS.bmsDclTemperature | BMS.bmsCclHighSoc | BMS.bmsCclTemperature)) !== 0;
			break;

		case 'bitfieldIO':
			updateField("brakeHold"                     , FieldClass.ok, bitfield & IO.brakeHold);
			updateField("preChargeRelay"                , FieldClass.ok, bitfield & IO.preChargeRelay);
			updateField("secondaryContactor"            , FieldClass.ok, bitfield & IO.secondaryContactor);
			updateField("mainContactor"                 , FieldClass.ok, bitfield & IO.mainContactor);
			updateField("enableMotor"                   , FieldClass.ok, bitfield & IO.enableMotor);
			updateField("coolingFan"                    , FieldClass.ok, bitfield & IO.coolingFan);
			updateField("brakeLight"                    , FieldClass.ok, bitfield & IO.brakeLight);
			updateField("reverseLight"                  , FieldClass.ok, bitfield & IO.reverseLight);
			updateField("enableIn"                      , FieldClass.ok, bitfield & IO.enableIn);
			updateField("absActive"                     , FieldClass.ok, bitfield & IO.absActive);
			break;
		}

		autoOpenClose(warningMotor || warningBms);
	}
	
	function updateField(id, fieldClass, flag) {
		$('#' + id).attr('class', flag == 0 ? FieldClass.off : fieldClass);
	}

	function autoOpenClose(warning) {
		if (warning) {
			if (foldTimeout) {
				clearTimeout(foldTimeout);
				foldTimeout = null;
			}
			if(!openedAutomatically) {
				fold(true);
				openedAutomatically = true;
			}
		} else if (openedAutomatically && foldTimeout == null) {
			// auto-fold after 10sec no error or warning
			foldTimeout = setTimeout(function () {
				fold(false);
				openedAutomatically = false;
				foldTimeout = null;
			}, 10000);
		}
	}

	function fold(open) {
		var annunciators = $('#annunciators');
		if (annunciators) {
			if(open == undefined) {
				open = annunciators.attr('class') !== 'fly-in';
			}
			annunciators.attr('class', (open ? 'fly-in' : 'fly-out'));
		}
	}
})();
