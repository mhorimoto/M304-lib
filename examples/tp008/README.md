# 十字キースイッチのサブルーティン

十字キーのインタフェースを容易に使うためのサブルーティン。

## 1. 基本ルーティン

### 1-1. 汎用キー入力ルーティン
#### struct KYBDMEM *getCrossKey(void)

呼び出すたびに押されているボタンスイッチの値とSelectorのアナログ値を返す。  
値は以下の構造体に納めて返される。
表示は行わられない。

```C++
struct KYBDMEM {
  uint8_t   kpos;
  uint16_t  selpos;
};
```
Table. 1  kposの意味
|    kpos       |   意味    |
|---------------|-----------|
|- - - - - - - 1| 決定 K_ENT|
|- - - - - - 1 -| ↑ K_UP |
|- - - - - 1 - -| ↓ K_DOWN|
|- - - - 1 - - -| ← K_LEFT|
|- - - 1 - - - -| → K_RIGHT|

Table.2 selposの値
| selpos |  0〜0x3ff |
|---------|----------|

### 1-2. 数値入力ルーティン
#### boolean getNumberValue(int x0,int y0,uint8_t base,long min,long max,uint8_t cast,uint8_t fkey,uint8_t akey,long *lgval )
#### parameters
- x0: 入力表示を開始するx座標 0〜19
- y0: 入力表示を開始するy座標 0〜3
- base: 表示に用いる進数(2,8,10,16)
- min: A/D出力0のときの値
- max: A/D出力0x3FFのときの値
- cast: 表示桁数(右詰め空白パディング)
- fkey: 入力確定キー
- akey: 入力中断キー
- lgval: 初期値を入れてコールする。確定した数値が戻ってくる。call by referenceで呼び出す。
- 
#### return
入力正常終了時には戻り値としてTRUEが返る。そして、範囲変換後の数値がlong型としてlgvalに格納される。
入力中断時には戻り値としてFALSEが返る。そして、

## 2. 応用ルーティン

### 2-1. IPアドレス入力ルーティン
#### boolean getIPaddrByCrossKey(int x0,int y0,const char *title,int x1,int y1,struct IPADDR *ip)

#### parameters
- x0: titleを表示するx座標 0〜19
- y0: titleを表示するy座標 0〜3
- title: タイトル
- x1: 入力を行うx座標 0〜4
- y1: 入力を行うy座標 0〜3
- ip: IPアドレス初期値および決定結果
```C++
struct IPADDR {
  uint8_t   addr[4];
} *inet;
```
```C++
 inet->addr[0] = 192;
 inet->addr[1] = 168;
 inet->addr[2] = 178;
 inet->addr[3] = 100;
```
#### return
- IPアドレスを変更設定した: TRUE
- 変更せずに終了: FALSE

IPアドレス (192.168.178.100 など)の設定を行う。
桁の移動は左右キーで行う。各桁の数値は、selposで行う。
決定したら「決定」を押す。上下キーを押すと変更せずに終了する。
