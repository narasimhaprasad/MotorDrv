1kW motor drive with active power factor correction

PIN			Function		    Variable
PB6			SPWM			    inv.cycle
PB7			SPWM'			    	-
PA6			PWM				    conv.cycle
PE0			ADC Ch3			    adc.ovoltage, adc.buff
PE1			ADC	Ch2			    adc.ocurrent, adc.buff
PE2			ADC Ch1			    adc.ivoltage, adc.buff
PF1			Debug			    
			Voltage PID Kp      vpid.kp
			Voltage PID Ki      vpid.ki
			Voltage PID ref     vpid.ref
			Current PID Kp      ipid.kp
			Current PID Ki      ipid.ki
			Current PID ref     ipid.ref
			

			
Note all control has been performed in Q13.19 format
Resolution : 1.907e-6
Data range : -4905 to 4905

Author : Narasimha Prasad Prabhu

Licence: CC sharealike non-commercial