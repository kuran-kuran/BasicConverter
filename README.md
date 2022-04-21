# MZ-80B/2000 BASICコンバータ 0.9.0
SHARP MZ-80B/2000のBASICをC++言語に変換します。VisualStudio2019でビルドするとWindowsで動作するexeファイルを作成することができます。

https://github.com/kuran-kuran/BasicConverter

## はじめに
このプログラムはMZ-80B/2000用のBASICで作られたプログラムをC++言語に変換します

## 対応BASIC
### MZ-80B
- SB-5520
### MZ-2000/2200
- MZ-1Z001

BASIC中でUSR関数などでマシン語を呼んでいる場合は実行してもうまく動作しません。

## ファイル構成
```
-\
+-\SBasicConverter        : BASICコンバータ一式
| +-- SBasicConverter.exe : BASICコンバータ
+-\SBasicExecuter         : BASICビルド用のプロジェクト一式
| +-\ library             : png読み込み用のライブラリ一式
| +-\ Project             : Basic.cppをビルドするためのVisualStudio2019用ソリューションファイル一式
| | +-\ vs2019
| |   +- mz2000font.png   : フォントファイル
| +-\ source              : Basic.cppを動作させるためのプログラムソース一式
+-LICENSE                 : ライセンスファイル
+-README.md               : このファイル
```

## 使い方
SBasicConverter.exeにmztファイルをドロップしてください。
するとBasic.cppファイルとConverter.logファイルが作成されます。
エラーチェックなどは一切行っていないためlogファイルは今のところ全く意味がありません。

## Basic.cppのビルド方法
SBasicExecuter/sourceフォルダにBasic.cppをコピーして
VisualStudio2019でExecBasic.slnファイルを開きビルドしてください。
正常にコンバートできていればビルドできますが
正常にコンバートできてない場合はエラーが出ます。
その場合はプログラムを自分で直すかあきらめてください。
作成したexeファイルを実行するときはexeと同じフォルダにmz2000font.pngを置いてください。

## 変更履歴
2022/04/21 ver.0.9.0
- ある程度動作したので公開した。

## 既知のバグ
- USR($0F14)でBEEP音が鳴らない。
- 計算式中の比較=を==に変換できない。
- RESTOREの行番号先にDATA文が無いとビルドできない。
- GOTOの行番号先にプログラムが無いとビルドできない。
- CLRしても配列が初期化されない。

## ライセンス
SBasicConverterおよびSBasicExecuterはMIT Licenseとします。
また、SBasicExecuterはlpngおよびzlibを使用しています。
そちらのライセンスはSBasicExecuter\libraryフォルダのLICENSEファイルをご覧ください。

## 作者について
- Twitter

https://twitter.com/kuran_kuran

- アルゴの記憶

https://daimonsoft.info/argo/
