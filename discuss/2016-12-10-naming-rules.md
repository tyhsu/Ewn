# 變數規則 
### Class

   - 大寫開頭
   - ex: `Chess` , `Game`, `Movable_chs_map`
	 
### Function
   - 小寫開頭
   - 以動詞為開頭
   - bool判斷=>前面加入 `is_`
   - ex: `roll_dice`, `print_board`

### 全域變數
   - 一般底線派
	 
### 區域變數 
   - 一般底線派
   - 重複使用的變數名稱後面加入 `_` <br> ex: `turn_`, `symbol_` ...
	 
### member variable
   - 統一用 `this` 來表示

### 命名脈絡
   - 去蕪存菁，簡短為主 <br> ex: `chs`, `direct`, `cur` , `pos`
   - 當前=> + `cur`
   - 陣列=>後面加入 `list`
   - 位元運算=>後面加入 `bitwise`
   - 陣列索引(int型態)=>後面加入 `index`
   - 去除冠詞 `roll_the_dice` => `roll_dice`
   - chess 的 symbol(char型態)=>後面加入 `symbol`
