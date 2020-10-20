var annunciator = annunciator || {};

(function() {
	annunciator.update = update;
	annunciator.fold = fold;

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
	var errorMotor = false;
	var errorBms = false;

	function update(name, bitfield) {
		switch (name) {
		case 'bitfieldMotor':
			updateField('warning'                       , 'warn', bitfield & Motor.warning);
			updateField('oscillationLimiter'            , 'ok', bitfield & Motor.oscillationLimiter);
			updateField('maximumModulationLimiter'      , 'warn', bitfield & Motor.maximumModulationLimiter);

			updateField('overtempController'            , 'error', bitfield & Motor.overtempController);
			updateField('overtempMotor'                 , 'error', bitfield & Motor.overtempMotor);
			updateField('overspeed'                     , 'error', bitfield & Motor.overspeed);
			updateField('hvUndervoltage'                , 'error', bitfield & Motor.hvUndervoltage);
			updateField('hvOvervoltage'                 , 'error', bitfield & Motor.hvOvervoltage);
			updateField('hvOvercurrent'                 , 'error', bitfield & Motor.hvOvercurrent);
			updateField('acOvercurrent'                 , 'error', bitfield & Motor.acOvercurrent);

			updateField('limitationTorque'              , 'warn', bitfield & Motor.limitationTorque);
			updateField('limitationMaxTorque'           , 'warn', bitfield & Motor.limitationMaxTorque);
			updateField('limitationSpeed'               , 'warn', bitfield & Motor.limitationSpeed);
			updateField('limitationControllerTemperature', 'warn', bitfield & Motor.limitationControllerTemperature);
			updateField('limitationMotorTemperature'    , 'warn', bitfield & Motor.limitationMotorTemperature);
			updateField('limitationSlewRate'            , 'warn', bitfield & Motor.limitationSlewRate);
			updateField('limitationMotorModel'          , 'warn', bitfield & Motor.limitationMotorModel);
			updateField('limitationMechanicalPower'     , 'warn', bitfield & Motor.limitationMechanicalPower);
			updateField('limitationAcVoltage'           , 'warn', bitfield & Motor.limitationAcVoltage);
			updateField('limitationAcCurrent'           , 'warn', bitfield & Motor.limitationAcCurrent);
			updateField('limitationDcVoltage'           , 'warn', bitfield & Motor.limitationDcVoltage);
			updateField('limitationDcCurrent'           , 'warn', bitfield & Motor.limitationDcCurrent);

			errorMotor = (bitfield & (Motor.overtempController | Motor.overtempMotor | Motor.overspeed |
					Motor.hvUndervoltage | Motor.hvOvervoltage | Motor.hvOvercurrent | Motor.acOvercurrent)) !== 0;
			break;

		case 'bitfieldBms':
			updateField('bmsRelayDischarge'             , 'ok', bitfield & BMS.bmsRelayDischarge);
			updateField('bmsRelayCharge'                , 'ok', bitfield & BMS.bmsRelayCharge);
			updateField('bmsChagerSafety'               , 'ok', bitfield & BMS.bmsChagerSafety);
			updateField('bmsDtcPresent'                 , 'error', bitfield & BMS.bmsDtcPresent);

			updateField('bmsVoltageFailsafe'            , 'error', bitfield & BMS.bmsVoltageFailsafe);
			updateField('bmsCurrentFailsafe'            , 'error', bitfield & BMS.bmsCurrentFailsafe);
			updateField('bmsDepleted'                   , 'error', bitfield & BMS.bmsDepleted);
			updateField('bmsBalancingActive'            , 'ok', bitfield & BMS.bmsBalancingActive);
			updateField('bmsDtcWeakCellFault'           , 'error', bitfield & BMS.bmsDtcWeakCellFault);
			updateField('bmsDtcLowCellVolage'           , 'error', bitfield & BMS.bmsDtcLowCellVolage);
			updateField('bmsDtcHVIsolationFault'        , 'error', bitfield & BMS.bmsDtcHVIsolationFault);
			updateField('bmsDtcVoltageRedundancyFault'  , 'error', bitfield & BMS.bmsDtcVoltageRedundancyFault);

			updateField('bmsDclLowSoc'                  , 'warn', bitfield & BMS.bmsDclLowSoc);
			updateField('bmsDclHighCellResistance'      , 'warn', bitfield & BMS.bmsDclHighCellResistance);
			updateField('bmsDclTemperature'             , 'warn', bitfield & BMS.bmsDclTemperature);
			updateField('bmsDclLowCellVoltage'          , 'warn', bitfield & BMS.bmsDclLowCellVoltage);
			updateField('bmsDclLowPackVoltage'          , 'warn', bitfield & BMS.bmsDclLowPackVoltage);
			updateField('bmsDclCclVoltageFailsafe'      , 'warn', bitfield & BMS.bmsDclCclVoltageFailsafe);
			updateField('bmsDclCclCommunication'        , 'warn', bitfield & BMS.bmsDclCclCommunication);
			updateField('bmsCclHighSoc'                 , 'warn', bitfield & BMS.bmsCclHighSoc);
			updateField('bmsCclHighCellResistance'      , 'warn', bitfield & BMS.bmsCclHighCellResistance);
			updateField('bmsCclTemperature'             , 'warn', bitfield & BMS.bmsCclTemperature);
			updateField('bmsCclHighCellVoltage'         , 'warn', bitfield & BMS.bmsCclHighCellVoltage);
			updateField('bmsCclHighPackVoltage'         , 'warn', bitfield & BMS.bmsCclHighPackVoltage);
			updateField('bmsCclChargerLatch'            , 'warn', bitfield & BMS.bmsCclChargerLatch);
			updateField('bmsCclAlternate'               , 'warn', bitfield & BMS.bmsCclAlternate);

			errorBms = (bitfield & (BMS.bmsVoltageFailsafe | BMS.bmsCurrentFailsafe | BMS.bmsDepleted |
					BMS.bmsDtcWeakCellFault | BMS.bmsDtcLowCellVolage | BMS.bmsDtcHVIsolationFault |
					BMS.bmsDtcVoltageRedundancyFault)) !== 0;
			break;

		case 'bitfieldIO':
			updateField('brakeHold'                     , 'ok', bitfield & IO.brakeHold);
			updateField('preChargeRelay'                , 'ok', bitfield & IO.preChargeRelay);
			updateField('secondaryContactor'            , 'ok', bitfield & IO.secondaryContactor);
			updateField('mainContactor'                 , 'ok', bitfield & IO.mainContactor);
			updateField('enableMotor'                   , 'ok', bitfield & IO.enableMotor);
			updateField('coolingFan'                    , 'ok', bitfield & IO.coolingFan);
			updateField('brakeLight'                    , 'ok', bitfield & IO.brakeLight);
			updateField('reverseLight'                  , 'ok', bitfield & IO.reverseLight);
			updateField('enableIn'                      , 'ok', bitfield & IO.enableIn);
			updateField('absActive'                     , 'ok', bitfield & IO.absActive);
			break;
		}

		autoOpenClose(errorMotor || errorBms);
	}
	
	function updateField(id, fieldClass, flag) {
		$('#' + id).attr('class', flag == 0 ? 'off' : fieldClass);
	}

	function autoOpenClose(error) {
		if (error) {
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
