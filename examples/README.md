# Test Programs (tp)

M304 を単体動作させて機能を確認するためのプログラムです．

## tp001

LCDの動作確認を行うプログラム

## tp002

矢印キーおよび選択ボリュームの動作確認を行うプログラム

## tp003

選択ボリュームの出力値 0〜1023 を 0〜255 にまるめて，その値を元にリレーを動作させる．
すなわち，8個あるリレーをボリューム出力値に合わせてMake/Breakさせる．

## tp004

I2C EEPROMの動作確認を行うプログラム．  
実体は，ardeeprom (https://github.com/mhorimoto/ardeeprom.git) である．

## tp005

Ethernetを用いたLAN接続動作確認を行うプログラム

http://www.holly-linux.com/whoami/

へアクセスして，自局のIPアドレスを表示します．DHCPサービスでローカルIPが必要になります．

## tp006

RTCを用いた時刻表示プログラム

* DS1307RTC Library (https://github.com/PaulStoffregen/DS1307RTC.git)
* Time Library (https://github.com/PaulStoffregen/Time)

## tp007

GPSを用いた時刻表示ブログラム  
GPSで得られた時刻とRTCで得られた時刻を並行表示する．

## tp100

一連のTPシリーズとは異なりTinyGSMを用いてLTE回線経由でインターネットアクセスする．

* TinyGSM Library (https://github.com/vshymanskyy/TinyGSM.git)

WebClientを試す．

