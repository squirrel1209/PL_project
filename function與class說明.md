# 語法分析器說明文件

本文件提供對語法分析器中各個組件的詳細說明，包括類型定義、錯誤處理結構以及核心函數。

## 類型定義

本節描述了語法分析器中使用的各種類型。

### Type Enum

`Type` 枚舉定義了不同的標記或符號類型，用於識別和分類語法元素。

- `NONE`: 未知類型
- `SEMICOLON`: 分號符號 ';'
- `ASSIGN`: 賦值符號 ':='
- `LESSEQUAL`: 小於等於符號 '<='
- `GREATEREQUAL`: 大於等於符號 '>='
- `LESS`: 小於符號 '<'
- `GREATER`: 大於符號 '>'
- `NOTEQUAL`: 不等於符號 '<>'
- `EQUAL`: 等於符號 '='
- `MINUS`: 減號 '-'
- `PLUS`: 加號 '+'
- `DIVIDE`: 除號 '/'
- `MULTIPLY`: 乘號 '*'
- `RPAREN`: 右括號 ')'
- `LPAREN`: 左括號 '('
- `IDENT`: 標識符
- `INT`: 整數
- `FLOAT`: 浮點數
- `BOOL`: 布爾類型
- `SIGN`: 符號（未來擴展用）
- `ERROR`: 錯誤
- `QUIT`: 退出

### ErrorType Enum

`ErrorType` 枚舉定義了可能發生的錯誤類型。

- `LEXICALERROR`: 詞法錯誤
- `SYNTACTICALERROR`: 語法錯誤
- `SEMANTICERROR`: 語義錯誤
- `NONEERROR`: 無錯誤

### Error 結構體

`Error` 結構體用於表示錯誤的類型和相關訊息。

### Token 結構體

`Token` 結構體包含標記的名稱、類型、所在行數和錯誤訊息。

## 核心函數

### CreateToken

`CreateToken` 函數用於創建一個新的 `Token` 實例。

### AnyToString 函數

一組 `AnyToString` 函數用於將不同類型的數據轉換為字符串。

### booleanExp 函數

`booleanExp` 函數用於解析布林表達式，這類表達式通常涉及至少一個算術表達式和一個布林運算符來進行比較。

### IDlessArithExpOrBexp 函數

`IDlessArithExpOrBexp` 函數用於處理不包含標識符開頭的算術或布林表達式，透過運算符將表達式分成左右操作數進行運算。

### NOT_ID_StartFactor 函數

`NOT_ID_StartFactor` 函數用於解析不以標識符開頭的因子。這包括處理數字、括號內的表達式，以及負數等情況。

### NOT_ID_StartTerm 函數

`NOT_ID_StartTerm` 函數用於解析不以標識符開頭的 term。這涉及到處理算術運算中的乘法和除法運算。

### NOT_ID_StartArithExp 函數

`NOT_ID_StartArithExp` 函數用於解析不以標識符開頭的算術表達式，確定表達式的結構並計算其值。


### NOT_IDStartArithExpOrBexp 函數

`NOT_IDStartArithExpOrBexp` 函數用於解析非標識符開頭的算術或布爾表達式，並根據解析結果更新 `parsedResult`。


### command 函數

`command` 函數負責解析並處理當前命令，根據命令的內容更新 `parsedResult` 變數。

### parse 函數

`parse` 函數負責執行整個解析過程，讀取並處理輸入的 token，直到遇到終止指令為止。

### processTokens 函數

`processTokens` 函數用於處理和分析輸入中的 tokens，給每個 token 賦予適當的類型。

### checkError 函數

`checkError` 函數用於檢查給定的字符串，判斷其是否包含任何語法錯誤，並返回一個包含錯誤信息的 `Error` 結構體。

### analyzeToken 函數

`analyzeToken` 函數用於分析給定的 token 字串，並根據其值返回相應的 `Type` 枚舉類型。

### splitString 函數

`splitString` 函數用於將一個字符串根據第二個出現的點 ('.') 分割成左右兩部分。這個分割過程忽略第三個點及之後的所有字符。

### isSIGN 函數

`isSIGN` 函數用於判斷提供的字符串是否對應於有效的數學符號，可以是加號 ("+") 或減號 ("-")。

### isInt 函數

`isInt` 函數用於檢查一個字符串是否可以表示為一個有效的整數。

### isNUM 函數

`isNUM` 函數用於檢查給定的字符串是否可以表示為一個有效的數字，包括整數和浮點數。該函數檢查字符串是否只包含數字和最多一個小數點。

### isMultiFloat 函數

`isMultiFloat` 函數檢查一個字符串是否表示多個由小數點分隔的浮點數。這對於解析特定格式的數據非常有用。


### isIDENT 函數

`isIDENT` 函數用於檢查給定的字符串是否符合標識符的命名規則。在多數編程語言中，標識符用於變量名、函數名等，且有嚴格的命名規則。

### isDelimiter 函數

`isDelimiter` 函數用於檢查給定的字符是否是一個分隔符。該函數支持常見的單字符分隔符，並對特定的雙字符分隔符進行特殊處理。

### getDelimiter 函數

`getDelimiter` 函數用於識別並返回基於當前字符的分隔符或操作符。該函數處理單字符和雙字符的操作符，並在必要時從輸入中獲取額外的字符以形成完整的分隔符。

### getNextToken 函數

`getNextToken` 函數負責識別並返回下一個有效的 token。這個函數處理空白字符，跳過註釋，並區分分隔符與普通字符序列。

### readRemainingToken 函數

`readRemainingToken` 函數負責從當前位置開始讀取字符，直到遇到空白字符或分隔符，從而形成一個完整的 token。這個函數對於分詞（tokenization）過程中的字符串提取特別重要。

### getNextNonWhiteSpaceChar 函數

`getNextNonWhiteSpaceChar` 函數用於從輸入文本中獲取下一個非空白字符。該函數首先調用 `getNextChar` 函數來讀取下一個字符，如果讀取的字符是空白（如空格、換行等），則繼續讀取下一個字符，直至找到非空白字符為止。這個函數對於忽略源代碼中的空白字符非常有用，有助於簡化後續的語法分析過程。

### getNextChar 函數

`getNextChar` 函數從輸入文本中獲取下一個字符。

## Tokenizer 類

`Tokenizer` 類封裝了詞法分析的邏輯，提供字符讀取和處理的功能。


