
EBNF Description of the Pico Min command language:
Although some productions are given in mixed case, all commands are actually processed in a case-insensitive manner.

pico_min_command	= get_command		|		#retrieve various data from the system
					  reset_command		|		#reboots MCU
					  set_command 		|		#set filament or tube voltage or current targets

get_command			= get_command_word (system_data | info_data | temperature_data); 
init_command		= init_command_word
reset_command		= reset_command_word;
set_command			= set_command_word ;

get_command_word	= "get" | "g";
reset_command_word	= "reset";
set_command_word	= "set" | "s";

system_data			= "sys" | "system";

info_data			 = info [decode];
info 				 = "info" | "information";	# display initial start-up info
decode				 = "decode";				# decode encoded build info

temperature_data	 = temp [temp_units] [temp_details];
temp 				 = "temp" | "temperature";
temp_units			 = ("kelvin" | "kel") | ("celsius" | "cel") | ("fahrenheit" | "fahr" | "fah" | "far");
temp_details		 = "details";

simple_report        = summary_report | ops_report | details_report;

summary_report       = "summary";
ops_report           = "ops_data" | "opsdata" | "ops";
details_report       = "details";

all_key_word         = "all";

temperature_name				= temp_sensor_name | all_key_word;

target_value					= value units;
units							= voltage_units | current_units | dac_units;
dac_units						= "c" | "counts";
current_units 					= "a" | "amps" | "ma" | "milliamps" | "ua" | "microamps";
voltage_units 					= "v" | "volts" | "kv" | "kilovolts";

value							= ["+" | "-"] decimal_number;
decimal_number					= digit {digit} ["." {digit}] | "." digit {digit};

# Note 1: Scientific notation (e.g., 1.23e-4) is supported via C++ parsing, 
# though not explicitly modeled in this grammar.
# Note 2: All numeric values are handled as c++ float types. In cases where
# integer values are implicit, such as with dac_units, or 'counts', the value is rounded to the nearest integer.

digit = "0" | non_zero_digit;
non_zero_digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";

--------------
Examples:

