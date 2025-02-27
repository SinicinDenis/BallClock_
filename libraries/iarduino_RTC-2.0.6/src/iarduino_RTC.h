//	Библиотека для работы с часами реального времени: (на чипе DS1302) https://iarduino.ru/shop/Expansion-payments/rtc-modul-ds1302.html
//                                                    (на чипе DS1307) https://iarduino.ru/shop/Expansion-payments/kroshechnye-rtc-modul-realnogo-vremeni.html
//                                                                     https://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul.html
//                                                    (на чипе DS3231) https://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-ds3231.html
//                                                                     https://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul-v2-0.html
//                                                    (на чипе RX8025)
//  Версия: 2.0.6
//  Последнюю версию библиотеки Вы можете скачать по ссылке: https://iarduino.ru/file/235.html
//  Подробное описание функции бибилиотеки доступно по ссылке: https://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: https://iarduino.ru
//  Автор библиотеки: Панькин Павел
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_RTC_h																						//
#define iarduino_RTC_h																						//
																											//
#define	RTC_UNDEFINED 0																						//	Модуль часов реального времени не определён
																											//
#if defined(ARDUINO) && (ARDUINO >= 100)																	//
#include <Arduino.h>																						//
#else																										//
#include <WProgram.h>																						//
#endif																										//
																											//
#include "iarduino_RTC_I2C.h"																				//	Подключаем библиотеку выбора реализации шины I2C.
#include "memorysaver.h"																					//	Подключаем файл «хранитель памяти»									(внутри файла есть комментарий поясняющий как сэкономить мапять)
																											//
#if defined(TwoWire_h) || defined(__ARDUINO_WIRE_IMPLEMENTATION__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega2560__) || defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(RENESAS_CORTEX_M4) // Если подключена библиотека Wire или платы её поддерживают...
#include <Wire.h>																							//	Разрешаем использовать библиотеку Wire в данной библиотеке.
#endif																										//
#if defined( iarduino_I2C_Software_h )																		//	Если библиотека iarduino_I2C_Software подключена в скетче...
#include <iarduino_I2C_Software.h>																			//	Разрешаем использовать библиотеку iarduino_I2C_Software в данной библиотеке.
#endif																										//
																											//
class iarduino_RTC_BASE{																					//	Объявляем полиморфный класс
	public:																									//
		virtual void	begin				(iarduino_I2C_Select*);											//	Объявляем функцию инициализации модуля								(без параметров)
		virtual uint8_t	funcReadTimeIndex	(uint8_t);														//	Объявляем функцию чтения 1 значения из регистров даты и времени		(0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели)
		virtual void	funcWriteTimeIndex	(uint8_t, uint8_t);												//	Объявляем функцию записи 1 значения в  регистры  даты и времени		(0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели, значение)
};																											//
																											//
#include "iarduino_RTC_DS1302.h"																			//	Подключаем файл iarduino_RTC_DS1302.h
#include "iarduino_RTC_DS1307.h"																			//	Подключаем файл iarduino_RTC_DS1307.h
#include "iarduino_RTC_DS3231.h"																			//	Подключаем файл iarduino_RTC_DS3231.h
#include "iarduino_RTC_RX8025.h"																			//	Подключаем файл iarduino_RTC_RX8025.h
																											//
class iarduino_RTC{																							//
	public:																									//
	/**	Конструктор класса **/																				//
		iarduino_RTC(uint8_t i, uint8_t j=SS, uint8_t k=SCK, uint8_t n=MOSI){								//	Конструктор основного класса										(название [, вывод SS/RST [, вывод SCK/CLK [, вывод MOSI/DAT]]])
			switch(i){																						//	Тип выбранного модуля
				#ifdef RTC_ENABLE_DS1302																	//
				case RTC_DS1302: objClass = new iarduino_RTC_DS1302(j,k,n); break;							//	Если используется модуль на базе чипа DS1302, то присваиваем указателю objClass ссылку на новый объект производного класса iarduino_RTC_DS1302 переопределяя на него все виртуальные функции полиморфного класса iarduino_RTC_BASE
				#endif																						//
				#ifdef RTC_ENABLE_DS1307																	//
				case RTC_DS1307: objClass = new iarduino_RTC_DS1307; break;									//	Если используется модуль на базе чипа DS1307, то присваиваем указателю objClass ссылку на новый объект производного класса iarduino_RTC_DS1307 переопределяя на него все виртуальные функции полиморфного класса iarduino_RTC_BASE
				#endif																						//
				#ifdef RTC_ENABLE_DS3231																	//
				case RTC_DS3231: objClass = new iarduino_RTC_DS3231; break;									//	Если используется модуль на базе чипа DS3231, то присваиваем указателю objClass ссылку на новый объект производного класса iarduino_RTC_DS3231 переопределяя на него все виртуальные функции полиморфного класса iarduino_RTC_BASE
				#endif																						//
				#ifdef RTC_ENABLE_RX8025																	//
				case RTC_RX8025: objClass = new iarduino_RTC_RX8025; break;									//	Если используется модуль на базе чипа RX8025, то присваиваем указателю objClass ссылку на новый объект производного класса iarduino_RTC_RX8025 переопределяя на него все виртуальные функции полиморфного класса iarduino_RTC_BASE
				#endif																						//
			}	                 selI2C   = new iarduino_I2C_Select;										//	Переопределяем указатель selI2C на объект производного класса iarduino_I2C_Select.
		}																									//
	/**	Пользовательские функции **/																		//
		#if defined(TwoWire_h) || defined(__ARDUINO_WIRE_IMPLEMENTATION__)									//	Если подключена библиотека Wire.h
		bool begin (TwoWire* i=&Wire ){ selI2C->init(i); objClass->begin((iarduino_I2C_Select*)selI2C); gettime(); }	//	Определяем функцию инициализации модуля (Параметр: объект для работы с аппаратной шиной I2C).
		#endif																								//
		#if defined(iarduino_I2C_Software_h)																//	Если подключена библиотека iarduino_I2C_Software.h
		bool begin (SoftTwoWire* i   ){ selI2C->init(i); objClass->begin((iarduino_I2C_Select*)selI2C); gettime(); }	//	Определяем функцию инициализации модуля (Параметр: объект для работы с программной шиной I2C).
		#endif																								//
		void	period		(uint8_t i)				{valPeriod=i; valPeriod*=60000;}						//	Определяем функцию задания минимального периода обращения к модулю	(i = период в минутах)
		void	blinktime	(uint8_t i, float j=1)	{valBlink=i; valFrequency=uint32_t(1000/j);}			//	Определяем функцию позволяющую мигать одним из параметров времени	(i = 0-нет / 1-сек / 2-мин / 3-час / 4-день / 5-мес / 6-год / 7-день_недели / 8-полдень , j = частота мигания в Гц)
		void	gettime		(void)					{gettime("");}											//	Определяем функцию получения даты и времени из переменных			(без параметров)
		char*	gettime		(String);																		//	Объявляем  функцию «дублёр» получения даты и времени из переменных	(строка с параметрами)
		char*	gettime		(const char*);																	//	Объявляем  функцию получения даты и времени ввиде строки			(строка с параметрами)
		void	settime		(int, int=-1, int=-1, int=-1, int=-1, int=-1, int=-1);							//	Объявляем  функцию установки даты и времени							(сек, мин, час, день, мес, год, день_недели)
		void	settime		(String);																		//	Объявляем  функцию «дублёр» установки даты и времени из строки		(строка "Mon Oct 02 15:29:23 2023" или макрос __TIMESTAMP__)
		void	settime		(const char*);																	//	Объявляем  функцию установки даты и времени из строки				(строка "Mon Oct 02 15:29:23 2023" или макрос __TIMESTAMP__)
	   uint32_t	gettimeUnix	(void)					{gettime(""); return Unix;}								//	Определяем функцию получения cекунд прошедших с начала эпохи Unix	(без параметров)
		void	settimeUnix	(uint32_t);																		//	Объявляем  функцию установки cекунд прошедших с начала эпохи Unix	(сек)
		void	settimezone	(int8_t i)				{valTimeZone=i;}										//	Определяем функцию установки часового пояса							(±12)
																											//
	/**	Переменные доступные для пользователя **/															//
		uint8_t	seconds					=	0;																//	Секунды			0-59
		uint8_t	minutes					=	0;																//	Минуты			0-59
		uint8_t	hours					=	1;																//	Часы			1-12
		uint8_t	Hours					=	0;																//	Часы			0-23
		uint8_t	midday					=	0;																//	Полдень			0-1													(0-am, 1-pm)
		uint8_t	day						=	1;																//	День месяца		1-31
		uint8_t	weekday					=	0;																//	День недели		0-6													(0-воскресенье, 1-понедельник, ... , 6-суббота)
		uint8_t	month					=	1;																//	Месяц			1-12
		uint8_t	year					=	0;																//	Год				0-99												(без учёта века)
	   uint32_t	Unix					=	0;																//	Секунды			прошедшие с начала эпохи Unix						(01.01.1970 00:00:00 GMT)
																											//
	/**	Внутренние переменные **/																			//
		iarduino_RTC_BASE*					objClass;														//	Объявляем указатель на объект полиморфного класса					(функции данного класса будут переопределены, т.к. указателю будет присвоена ссылка на производный класс)
		iarduino_I2C_VirtualSelect*			selI2C;															//	Объявляем указатель на объект полиморфного класса					(функции данного класса будут переопределены, т.к. указателю будет присвоена ссылка на производный класс)
		char*	charReturn				=	(char*) malloc(1);												//	Определяем указатель на символьную область памяти в 1 байт			(указатель будет ссылаться на строку вывода времени)
  const char* 	charInput				=	"waAdhHimsyMDY";												//	Определяем константу-строку с символами требующими замены			(данные символы заменяются функцией gettime на значение времени)
  const char*	charMidday				=	"ampmAMPM";														//	Определяем константу-строку для вывода полудня						(am / pm / AM / PM)
  const char*	charDayMon				=	"SunMonTueWedThuFriSatJanFebMarAprMayJunJulAugSepOctNovDec";	//	Определяем константу-строку для вывода дня недели или месяца		(Mon ... Sun / Jan ... Dec)
		uint8_t	arrCalculationTime[7];																		//	Объявляем массив для рассчёта времени без обращения к модулю		(для хранения последних, прочитанных из модуля, значений даты и времени)
		uint8_t	valBlink				=	0;																//	Определяем параметр времени, который должен мигать					(1-сек / 2-мин / 3-час / 4-день / 5-мес / 6-год / 7-день_недели / 8-полдень)
	   uint32_t	valFrequency			=	1000;															//	Определяем частоту мигания параметра времени для функции blinktime	(по умолчанию 1 Гц)
		uint8_t	valCentury				=	21;																//	Определяем переменную для хранения текущего века					(по умолчанию 21 век)
	   uint16_t	valPeriod				=	0;																//	Определяем минимальный период опроса модуля							(в минутах, от 00 до 255)
	   uint32_t	valRequest				=	0;																//	Определяем время последнего чтения регистров времени
		int8_t	valTimeZone				=	0;																//	Часовой пояс для функций set/get timeUnix().						(в часах ±12)
	private:																								//
	/**	Внутренние функции **/																				//
		void	funcReadTime			(void);																//	Объявляем функцию чтения даты и времени из регистров модуля			(без параметров)
		void	funcWriteTime			(int, int, int, int, int, int, int);								//	Объявляем функцию записи даты и времени в  регистры  модуля			(без параметров)
		uint8_t	funcConvertCodeToNum	(uint8_t i)	{return (i >> 4)*10 + (i & 0x0F);}						//	Определяем функцию преобразования двоично-десятичного кода в число	(код)
		uint8_t	funcConvertNumToCode	(uint8_t i)	{return ((i/10) << 4) + (i%10);}						//	Определяем функцию преобразования числа в двоично-десятичный код	(число)
		void	funcSetMoreTime			(void){hours=(Hours%12)?(Hours%12):12; midday=(Hours<12)?0:1;}		//	Корректировка переменных не читаемых из модуля						(без параметров)
		void	funcCalculationTime		(void);																//	Объявляем функцию расчёта времени без обращения к модулю			(без параметров)
	   uint32_t	funcCalculationUnix		(void);																//	Объявляем функцию расчёта cекунд прошедших с начала эпохи Unix		(без параметров)
		void	funcFillChar			(uint8_t, uint8_t, uint8_t, uint8_t);								//	Объявляем функцию заполнения строки вывода времени					(данные, тип данных, позиция для вставки, мигание)
		int		funcDayMon				(const char*);														//	Объявляем функцию преобразования названия дня или месяца в число	(строка "Mon" ... "Sun" / "Jan" ... "Dec")
};																											//
																											//
#endif																										//