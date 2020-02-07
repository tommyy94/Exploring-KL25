EESchema Schematic File Version 4
LIBS:plantwatch-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L plantwatch-rescue:Mounting_Hole_PAD-Mechanical MK1
U 1 1 5A6CFED2
P 850 6600
F 0 "MK1" H 950 6651 50  0000 L CNN
F 1 "Mounting_Hole_PAD" H 950 6560 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_2.7mm" H 850 6600 50  0001 C CNN
F 3 "" H 850 6600 50  0001 C CNN
	1    850  6600
	1    0    0    -1  
$EndComp
$Comp
L plantwatch-rescue:Mounting_Hole_PAD-Mechanical MK2
U 1 1 5A6CFF98
P 850 6900
F 0 "MK2" H 950 6951 50  0000 L CNN
F 1 "Mounting_Hole_PAD" H 950 6860 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_2.7mm" H 850 6900 50  0001 C CNN
F 3 "" H 850 6900 50  0001 C CNN
	1    850  6900
	1    0    0    -1  
$EndComp
$Comp
L plantwatch-rescue:Mounting_Hole_PAD-Mechanical MK3
U 1 1 5A6CFFFA
P 850 7200
F 0 "MK3" H 950 7251 50  0000 L CNN
F 1 "Mounting_Hole_PAD" H 950 7160 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_2.7mm" H 850 7200 50  0001 C CNN
F 3 "" H 850 7200 50  0001 C CNN
	1    850  7200
	1    0    0    -1  
$EndComp
$Comp
L plantwatch-rescue:Mounting_Hole_PAD-Mechanical MK4
U 1 1 5A6D003F
P 850 7500
F 0 "MK4" H 950 7551 50  0000 L CNN
F 1 "Mounting_Hole_PAD" H 950 7460 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_2.7mm" H 850 7500 50  0001 C CNN
F 3 "" H 850 7500 50  0001 C CNN
	1    850  7500
	1    0    0    -1  
$EndComp
NoConn ~ 850  6700
NoConn ~ 850  7000
NoConn ~ 850  7300
NoConn ~ 850  7600
$Comp
L power:GND #PWR01
U 1 1 5E24FCBA
P 6200 6750
F 0 "#PWR01" H 6200 6500 50  0001 C CNN
F 1 "GND" H 6205 6577 50  0000 C CNN
F 2 "" H 6200 6750 50  0001 C CNN
F 3 "" H 6200 6750 50  0001 C CNN
	1    6200 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 6750 6200 6550
Wire Wire Line
	6000 6350 6000 6550
Wire Wire Line
	6000 6550 6200 6550
Connection ~ 6200 6550
Wire Wire Line
	6200 6550 6200 6350
Wire Wire Line
	6000 1150 6000 950 
Wire Wire Line
	6000 950  6200 950 
Wire Wire Line
	6200 950  6200 1150
Wire Wire Line
	6200 6550 6300 6550
Wire Wire Line
	6300 6550 6300 6350
Wire Wire Line
	6300 1150 6300 950 
Wire Wire Line
	6300 950  6200 950 
Connection ~ 6200 950 
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5E2CE1C5
P 10300 4600
F 0 "#FLG02" H 10300 4675 50  0001 C CNN
F 1 "PWR_FLAG" H 10300 4773 50  0000 C CNN
F 2 "" H 10300 4600 50  0001 C CNN
F 3 "~" H 10300 4600 50  0001 C CNN
	1    10300 4600
	-1   0    0    1   
$EndComp
NoConn ~ 4900 4750
NoConn ~ 4900 4650
$Comp
L MCU_NXP_Kinetis:MKL25Z128VLK4 U1
U 1 1 5E24E5F4
P 6100 3750
F 0 "U1" H 5450 1200 50  0000 C CNN
F 1 "MKL25Z128VLK4" H 5450 1100 50  0000 C CNN
F 2 "Package_QFP:LQFP-80_12x12mm_P0.5mm" H 6350 1325 50  0001 L TNN
F 3 "http://www.nxp.com/docs/en/data-sheet/KL25P80M48SF0.pdf" H 6400 1150 50  0001 L BNN
	1    6100 3750
	1    0    0    -1  
$EndComp
NoConn ~ 4900 5750
$Sheet
S 12500 0    12550 8250
U 5E3AB7F2
F0 "Sheet5E3AB7F1" 50
F1 "plantwatch_sheet2.sch" 50
$EndSheet
Text GLabel 7300 5550 2    50   Input ~ 0
HS1101
Text GLabel 4900 3350 0    50   Input ~ 0
TMP36GT
Text GLabel 4900 3450 0    50   Input ~ 0
SEN0193
NoConn ~ 4900 4450
NoConn ~ 4900 4350
NoConn ~ 4900 4250
NoConn ~ 4900 4150
NoConn ~ 4900 4050
NoConn ~ 4900 3950
NoConn ~ 4900 3850
NoConn ~ 4900 3750
NoConn ~ 4900 3650
NoConn ~ 4900 3550
NoConn ~ 7300 5750
NoConn ~ 7300 5650
NoConn ~ 7300 5450
NoConn ~ 7300 5350
NoConn ~ 7300 5250
NoConn ~ 7300 5150
NoConn ~ 7300 5050
NoConn ~ 7300 4950
NoConn ~ 7300 4850
NoConn ~ 7300 4350
NoConn ~ 7300 4150
NoConn ~ 7300 4050
NoConn ~ 7300 3950
NoConn ~ 7300 3850
NoConn ~ 7300 3750
NoConn ~ 7300 3650
NoConn ~ 7300 3550
NoConn ~ 4900 3050
NoConn ~ 4900 2750
NoConn ~ 4900 2650
NoConn ~ 4900 2550
NoConn ~ 4900 2450
NoConn ~ 4900 2350
NoConn ~ 4900 2250
NoConn ~ 4900 2150
NoConn ~ 5800 1150
NoConn ~ 7300 1750
NoConn ~ 7300 1850
NoConn ~ 7300 1950
NoConn ~ 7300 2050
NoConn ~ 7300 2150
NoConn ~ 7300 2250
NoConn ~ 7300 2350
NoConn ~ 7300 2450
NoConn ~ 7300 2550
NoConn ~ 7300 2650
NoConn ~ 7300 2750
NoConn ~ 7300 2850
NoConn ~ 7300 2950
NoConn ~ 7300 3050
NoConn ~ 7300 3150
NoConn ~ 7300 3250
Text GLabel 4900 1950 0    50   Input ~ 0
NRF24L01P_IRQ
Text GLabel 4900 1850 0    50   Input ~ 0
NRF24L01P_CE
Text GLabel 7300 4550 2    50   Input ~ 0
NRF24L01P_SCK
Text GLabel 7300 4750 2    50   Input ~ 0
NRF24L01P_CSN
Text GLabel 7300 4650 2    50   Input ~ 0
NRF24L01P_MOSI
Text GLabel 7300 4450 2    50   Input ~ 0
NRF24L01P_MISO
$Comp
L power:+3V3 #PWR0101
U 1 1 5E4E3216
P 6200 750
F 0 "#PWR0101" H 6200 600 50  0001 C CNN
F 1 "+3V3" H 6215 923 50  0000 C CNN
F 2 "" H 6200 750 50  0001 C CNN
F 3 "" H 6200 750 50  0001 C CNN
	1    6200 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 750  6200 950 
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J3
U 1 1 5E4F9447
P 1900 2000
F 0 "J3" H 1950 2417 50  0000 C CNN
F 1 "SWD" H 1950 2326 50  0000 C CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_2x05_P1.00mm_Vertical_SMD" H 1900 2000 50  0001 C CNN
F 3 "~" H 1900 2000 50  0001 C CNN
	1    1900 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5E4FA75F
P 1200 2150
F 0 "#PWR0104" H 1200 1900 50  0001 C CNN
F 1 "GND" H 1205 1977 50  0000 C CNN
F 2 "" H 1200 2150 50  0001 C CNN
F 3 "" H 1200 2150 50  0001 C CNN
	1    1200 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 1900 1200 1900
Wire Wire Line
	1200 1900 1200 2000
Wire Wire Line
	1700 2000 1200 2000
Connection ~ 1200 2000
Wire Wire Line
	1200 2000 1200 2150
$Comp
L power:+3V3 #PWR0105
U 1 1 5E4FE2DD
P 1200 1650
F 0 "#PWR0105" H 1200 1500 50  0001 C CNN
F 1 "+3V3" H 1215 1823 50  0000 C CNN
F 2 "" H 1200 1650 50  0001 C CNN
F 3 "" H 1200 1650 50  0001 C CNN
	1    1200 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 1800 1200 1800
Wire Wire Line
	1200 1800 1200 1650
NoConn ~ 1700 2100
NoConn ~ 1700 2200
NoConn ~ 2200 2100
NoConn ~ 2200 2000
Wire Wire Line
	2200 1900 3700 1900
Wire Wire Line
	3700 1900 3700 1750
Wire Wire Line
	3700 1750 4900 1750
Wire Wire Line
	4900 2050 3200 2050
Wire Wire Line
	3200 2050 3200 1800
Wire Wire Line
	3200 1800 2200 1800
Wire Wire Line
	1850 3250 1700 3250
$Comp
L Device:R_Small R1
U 1 1 5E522CB0
P 2100 3650
F 0 "R1" V 2200 3500 50  0000 L CNN
F 1 "10K" V 2200 3650 50  0000 L CNN
F 2 "Resistor_SMD:R_2512_6332Metric_Pad1.52x3.35mm_HandSolder" H 2100 3650 50  0001 C CNN
F 3 "~" H 2100 3650 50  0001 C CNN
	1    2100 3650
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5E522CB6
P 2250 3300
F 0 "#PWR06" H 2250 3050 50  0001 C CNN
F 1 "GND" H 2255 3127 50  0000 C CNN
F 2 "" H 2250 3300 50  0001 C CNN
F 3 "" H 2250 3300 50  0001 C CNN
	1    2250 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR05
U 1 1 5E522CBC
P 2450 3550
F 0 "#PWR05" H 2450 3400 50  0001 C CNN
F 1 "+3V3" H 2465 3678 50  0000 L CNN
F 2 "" H 2450 3550 50  0001 C CNN
F 3 "" H 2450 3550 50  0001 C CNN
	1    2450 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C8
U 1 1 5E522CC2
P 1950 3250
F 0 "C8" V 1850 3100 50  0000 L CNN
F 1 "1uF" V 1850 3250 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 1950 3250 50  0001 C CNN
F 3 "~" H 1950 3250 50  0001 C CNN
	1    1950 3250
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5E522CC9
P 1700 4500
F 0 "#PWR02" H 1700 4250 50  0001 C CNN
F 1 "GND" H 1850 4400 50  0000 C CNN
F 2 "" H 1700 4500 50  0001 C CNN
F 3 "" H 1700 4500 50  0001 C CNN
	1    1700 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 5E522CCF
P 1700 4100
F 0 "SW1" V 1750 4400 50  0000 C CNN
F 1 "Reset" V 1650 4400 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 1700 4300 50  0001 C CNN
F 3 "~" H 1700 4300 50  0001 C CNN
	1    1700 4100
	0    1    1    0   
$EndComp
Wire Wire Line
	2450 2200 2450 2500
Connection ~ 2450 2200
Wire Wire Line
	2450 2200 2200 2200
Wire Wire Line
	1700 4500 1700 4300
Connection ~ 1700 3250
Wire Wire Line
	2050 3250 2250 3250
Wire Wire Line
	2250 3250 2250 3300
Wire Wire Line
	2450 3650 2450 3550
Wire Wire Line
	2200 3650 2450 3650
Wire Wire Line
	1700 3250 1700 3650
Wire Wire Line
	1700 3650 2000 3650
Connection ~ 1700 3650
Wire Wire Line
	1700 3650 1700 3900
Wire Wire Line
	1700 2500 2450 2500
Wire Wire Line
	1700 2500 1700 3250
Wire Notes Line
	1300 4800 2750 4800
Wire Notes Line
	2750 4800 2750 2900
Wire Notes Line
	2750 2900 1300 2900
Wire Notes Line
	1300 2900 1300 4800
Text Notes 1900 3050 0    50   ~ 0
RESET BUTTON
Wire Notes Line
	900  1150 900  2600
Wire Notes Line
	900  2600 2600 2600
Wire Notes Line
	2600 2600 2600 1150
Wire Notes Line
	2600 1150 900  1150
Text Notes 1450 1350 0    50   ~ 0
SWD CONNECTOR
Text GLabel 7300 3450 2    50   Input ~ 0
MOTOR
$Comp
L Device:Crystal_GND24 Y1
U 1 1 5E35FF3C
P 3850 2600
F 0 "Y1" V 4150 2650 50  0000 L CNN
F 1 "8 MHz" V 4050 2250 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_0603-4Pin_6.0x3.5mm_HandSoldering" H 3850 2600 50  0001 C CNN
F 3 "~" H 3850 2600 50  0001 C CNN
	1    3850 2600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4900 2850 4750 2850
$Comp
L Device:R_Small R8
U 1 1 5E36D343
P 4500 2550
F 0 "R8" V 4600 2400 50  0000 L CNN
F 1 "1M" V 4600 2550 50  0000 L CNN
F 2 "Resistor_SMD:R_2512_6332Metric_Pad1.52x3.35mm_HandSolder" H 4500 2550 50  0001 C CNN
F 3 "~" H 4500 2550 50  0001 C CNN
	1    4500 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5E375FC1
P 3200 2850
F 0 "#PWR08" H 3200 2600 50  0001 C CNN
F 1 "GND" H 3205 2677 50  0000 C CNN
F 2 "" H 3200 2850 50  0001 C CNN
F 3 "" H 3200 2850 50  0001 C CNN
	1    3200 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2200 3000 3150
$Comp
L power:GND #PWR019
U 1 1 5E384D29
P 4250 2700
F 0 "#PWR019" H 4250 2450 50  0001 C CNN
F 1 "GND" H 4255 2527 50  0000 C CNN
F 2 "" H 4250 2700 50  0001 C CNN
F 3 "" H 4250 2700 50  0001 C CNN
	1    4250 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 2250 4500 2450
Connection ~ 4500 2250
Wire Wire Line
	4500 2250 4750 2250
Wire Wire Line
	3000 2200 2450 2200
Wire Wire Line
	3000 3150 4900 3150
$Comp
L Device:C_Small C12
U 1 1 5E39EB8D
P 3450 2800
F 0 "C12" H 3542 2846 50  0000 L CNN
F 1 "22pF" H 3542 2755 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 3450 2800 50  0001 C CNN
F 3 "~" H 3450 2800 50  0001 C CNN
	1    3450 2800
	1    0    0    -1  
$EndComp
Connection ~ 3450 2600
$Comp
L Device:C_Small C13
U 1 1 5E3A285B
P 3450 2400
F 0 "C13" H 3200 2400 50  0000 L CNN
F 1 "22pF" H 3150 2500 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 3450 2400 50  0001 C CNN
F 3 "~" H 3450 2400 50  0001 C CNN
	1    3450 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 2250 4500 2250
Wire Wire Line
	4050 2600 4250 2600
Wire Wire Line
	3450 2300 3450 2250
Wire Wire Line
	3200 2600 3450 2600
Wire Wire Line
	3450 2600 3650 2600
Wire Wire Line
	3450 2250 3850 2250
Connection ~ 3850 2250
Wire Wire Line
	3450 2950 3850 2950
Connection ~ 3850 2950
Wire Wire Line
	4250 2600 4250 2700
Wire Wire Line
	3850 2950 4500 2950
Wire Wire Line
	4500 2650 4500 2950
Connection ~ 4500 2950
Wire Wire Line
	4500 2950 4900 2950
Wire Wire Line
	4750 2250 4750 2850
Wire Wire Line
	3450 2900 3450 2950
Wire Wire Line
	3450 2700 3450 2600
Wire Wire Line
	3450 2500 3450 2600
Wire Wire Line
	3850 2250 3850 2450
Wire Wire Line
	3850 2750 3850 2950
Wire Wire Line
	3200 2600 3200 2850
Wire Wire Line
	9150 4250 9150 4300
$Comp
L power:GND #PWR022
U 1 1 5E401B64
P 9150 4300
F 0 "#PWR022" H 9150 4050 50  0001 C CNN
F 1 "GND" H 9155 4127 50  0000 C CNN
F 2 "" H 9150 4300 50  0001 C CNN
F 3 "" H 9150 4300 50  0001 C CNN
	1    9150 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 4250 9150 4250
$Comp
L power:GND #PWR025
U 1 1 5E45D1BC
P 9950 4600
F 0 "#PWR025" H 9950 4350 50  0001 C CNN
F 1 "GND" H 9955 4427 50  0000 C CNN
F 2 "" H 9950 4600 50  0001 C CNN
F 3 "" H 9950 4600 50  0001 C CNN
	1    9950 4600
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR026
U 1 1 5E45DB67
P 10500 3600
F 0 "#PWR026" H 10500 3450 50  0001 C CNN
F 1 "+3V3" H 10515 3773 50  0000 C CNN
F 2 "" H 10500 3600 50  0001 C CNN
F 3 "" H 10500 3600 50  0001 C CNN
	1    10500 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 4050 10500 4050
$Comp
L power:PWR_FLAG #FLG03
U 1 1 5E46382E
P 9950 3600
F 0 "#FLG03" H 9950 3675 50  0001 C CNN
F 1 "PWR_FLAG" H 9950 3773 50  0000 C CNN
F 2 "" H 9950 3600 50  0001 C CNN
F 3 "~" H 9950 3600 50  0001 C CNN
	1    9950 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 3600 9950 3750
Wire Wire Line
	9950 4350 9950 4450
Wire Wire Line
	10300 4450 10300 4600
Connection ~ 9950 4450
Wire Wire Line
	9950 4450 9950 4600
Wire Wire Line
	10500 3600 10500 4050
Wire Notes Line
	10900 3000 8300 3000
Text Notes 8500 3250 0    50   ~ 0
POWER SUPPLY
$Comp
L Connector:Barrel_Jack_Switch J6
U 1 1 5E36ADDA
P 8750 4150
F 0 "J6" H 8807 4467 50  0000 C CNN
F 1 "Power Input" H 8807 4376 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 8800 4110 50  0001 C CNN
F 3 "~" H 8800 4110 50  0001 C CNN
	1    8750 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 4150 9150 4150
Wire Wire Line
	9150 4150 9150 4250
Connection ~ 9150 4250
Wire Wire Line
	9950 4450 10300 4450
$Comp
L Regulator_Linear:NCP1117-3.3_SOT223 U4
U 1 1 5E442DD8
P 9950 4050
F 0 "U4" H 9950 4292 50  0000 C CNN
F 1 "+12V Regulator" H 9950 4201 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 9950 4250 50  0001 C CNN
F 3 "http://www.onsemi.com/pub_link/Collateral/NCP1117-D.PDF" H 10050 3800 50  0001 C CNN
	1    9950 4050
	1    0    0    -1  
$EndComp
Wire Notes Line
	8300 3000 8300 4950
Wire Notes Line
	8300 4950 10900 4950
Wire Notes Line
	10900 4950 10900 3000
$Comp
L Device:C_Small C2
U 1 1 5E25E894
P 8600 1550
F 0 "C2" H 8692 1596 50  0000 L CNN
F 1 "0.1uF" H 8692 1505 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 8600 1550 50  0001 C CNN
F 3 "~" H 8600 1550 50  0001 C CNN
	1    8600 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 1650 8600 1800
Wire Wire Line
	8800 1800 8600 1800
Connection ~ 8800 1800
Wire Wire Line
	8800 1350 9000 1350
Connection ~ 8800 1350
Wire Wire Line
	8800 1350 8800 1450
Wire Wire Line
	9000 1450 9000 1350
Wire Wire Line
	9000 1800 8800 1800
Wire Wire Line
	9000 1650 9000 1800
Wire Wire Line
	8800 1650 8800 1800
Wire Wire Line
	8600 1350 8800 1350
Wire Wire Line
	8600 1450 8600 1350
$Comp
L Device:C_Small C3
U 1 1 5E260057
P 8800 1550
F 0 "C3" H 8892 1596 50  0000 L CNN
F 1 "0.1uF" H 8892 1505 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 8800 1550 50  0001 C CNN
F 3 "~" H 8800 1550 50  0001 C CNN
	1    8800 1550
	1    0    0    -1  
$EndComp
Text Notes 8700 1000 0    50   ~ 0
DECOUPLING CAPACITORS
$Comp
L power:GND #PWR04
U 1 1 5E270BDD
P 9600 1900
F 0 "#PWR04" H 9600 1650 50  0001 C CNN
F 1 "GND" H 9605 1727 50  0000 C CNN
F 2 "" H 9600 1900 50  0001 C CNN
F 3 "" H 9600 1900 50  0001 C CNN
	1    9600 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C15
U 1 1 5E2A98D8
P 9600 1550
F 0 "C15" H 9692 1596 50  0000 L CNN
F 1 "0.1uF" H 9692 1505 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 9600 1550 50  0001 C CNN
F 3 "~" H 9600 1550 50  0001 C CNN
	1    9600 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 1800 9600 1650
Wire Wire Line
	9400 1800 9400 1650
$Comp
L Device:C_Small C6
U 1 1 5E2618C7
P 9400 1550
F 0 "C6" H 9492 1596 50  0000 L CNN
F 1 "0.1uF" H 9492 1505 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 9400 1550 50  0001 C CNN
F 3 "~" H 9400 1550 50  0001 C CNN
	1    9400 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 1350 9400 1450
Wire Wire Line
	9400 1800 9200 1800
Wire Wire Line
	9200 1650 9200 1800
Connection ~ 9000 1800
Connection ~ 9200 1800
Wire Wire Line
	9200 1800 9000 1800
Connection ~ 9000 1350
Wire Wire Line
	9000 1350 9200 1350
Wire Wire Line
	9200 1450 9200 1350
Connection ~ 9200 1350
Wire Wire Line
	9200 1350 9400 1350
Connection ~ 9400 1800
Wire Wire Line
	9600 1800 9400 1800
Wire Wire Line
	9600 1350 9600 1450
Connection ~ 9400 1350
Wire Wire Line
	9400 1350 9600 1350
Connection ~ 9600 1800
$Comp
L Device:C_Small C5
U 1 1 5E2618C1
P 9200 1550
F 0 "C5" H 9292 1596 50  0000 L CNN
F 1 "0.1uF" H 9292 1505 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 9200 1550 50  0001 C CNN
F 3 "~" H 9200 1550 50  0001 C CNN
	1    9200 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR03
U 1 1 5E271413
P 8600 1250
F 0 "#PWR03" H 8600 1100 50  0001 C CNN
F 1 "+3V3" H 8615 1423 50  0000 C CNN
F 2 "" H 8600 1250 50  0001 C CNN
F 3 "" H 8600 1250 50  0001 C CNN
	1    8600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 1350 8600 1250
$Comp
L Device:C_Small C1
U 1 1 5E3FBD17
P 9550 4300
F 0 "C1" H 9642 4346 50  0000 L CNN
F 1 "0.1uF" H 9642 4255 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 9550 4300 50  0001 C CNN
F 3 "~" H 9550 4300 50  0001 C CNN
	1    9550 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 4200 9550 4050
Wire Wire Line
	9550 4050 9650 4050
Connection ~ 9550 4050
$Comp
L power:+12V #PWR024
U 1 1 5E437894
P 9350 3600
F 0 "#PWR024" H 9350 3450 50  0001 C CNN
F 1 "+12V" H 9365 3773 50  0000 C CNN
F 2 "" H 9350 3600 50  0001 C CNN
F 3 "" H 9350 3600 50  0001 C CNN
	1    9350 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 3600 9350 3750
Connection ~ 9350 3750
Wire Wire Line
	9350 3750 9350 4050
Wire Wire Line
	9050 4050 9350 4050
Connection ~ 9350 4050
Wire Wire Line
	9350 4050 9550 4050
Wire Wire Line
	9350 3750 9950 3750
$Comp
L Device:C_Small C4
U 1 1 5E26029B
P 9000 1550
F 0 "C4" H 9092 1596 50  0000 L CNN
F 1 "0.1uF" H 9092 1505 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-15_AVX-H_Pad1.50x2.35mm_HandSolder" H 9000 1550 50  0001 C CNN
F 3 "~" H 9000 1550 50  0001 C CNN
	1    9000 1550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5E412603
P 9550 4600
F 0 "#PWR0102" H 9550 4350 50  0001 C CNN
F 1 "GND" H 9555 4427 50  0000 C CNN
F 2 "" H 9550 4600 50  0001 C CNN
F 3 "" H 9550 4600 50  0001 C CNN
	1    9550 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 4400 9550 4600
Wire Wire Line
	9600 1900 9600 1800
Wire Notes Line
	8200 2250 10050 2250
Wire Notes Line
	10050 2250 10050 800 
Wire Notes Line
	10050 800  8200 800 
Wire Notes Line
	8200 800  8200 2250
$EndSCHEMATC
