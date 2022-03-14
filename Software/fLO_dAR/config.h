"{\
	'general':\
	{\
		'name':'Device',\
		'instance':1,\
		'interval':5000,\
		'print_verbosity':2\
	},\
	'components':[\
		{\
			'name':'ADS1115',\
			'params':'default'\
		},\
    {\
      'name':'MAX31856',\
      'params':'default'\
    },\
		{\
			'name':'SD',\
			'params': 'default'\
		},\
    {\ 
​      'name':'DS3231',\ 
​      'params':[11, false, true]\ 
​    },\
		{\
			'name':'Interrupt_Manager',\
			'params':[0]\
		},\
		{\
			'name':'Sleep_Manager',\
			'params':[true,false,1]\
		}\
	]\
}"
