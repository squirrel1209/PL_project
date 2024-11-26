# C/C++ Interpreter Project

本專案是一個簡易的C/C++解釋器，它能夠解析並執行定義好的命令和表達式。

## 功能特點

- 支援基本算術表達式的解析和計算。
- 支援布林表達式的解析和計算。
- 支援變數賦值和使用。
- 支援註解。
- 支援互動式輸入和檔案輸入。

## 開發環境

- 開發語言：C/C++
- 建議編輯器：Visual Studio Code, CLion, 或任何支援C/C++的編輯器
- 編譯環境：GCC/G++, 支援C++11或以上標準

## 檔案結構

- `main.cpp`：主程式入口，負責讀取輸入和初始化解釋器。
- `Tokenizer.h`：負責將輸入文本分割成token。
   
- `Parser.h`：根據token進行語法分析並執行相應的命令。

## 使用方法

### 編譯專案

在專案目錄下執行以下命令來編譯專案：

```bash
g++ -std=c++11 -o interpreter main.cpp

## 範例
![alt text](./image.png)
   
