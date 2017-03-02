link:
https://hackmd.io/MwVgnMCmBGBMDGBaYBDELEBYBs2CMiKA7LAGaIAM8aKeo8s0eQA=?both

# 資工專題報告
棋逢敵手

---

## 指導教授
吳毅成

## 組員
許庭嫣 楊君亮 林立秦

---

## 遊戲簡介

愛因斯坦棋

----

## 遊戲簡介
#### 道具

* 骰子
* 棋盤（兩方各有棋子1~6）
![](https://i.imgur.com/XeqHGXd.jpg)

----

### 遊戲簡介
#### 規則

* 初始布置為將六枚己棋任意擺在棋盤己側左上角的3*3的三角型區域。
* 每方回合輪流擲骰子，然後選擇一枚與骰點同樣數字的己棋朝朝下、右、或右下方移動一格至任何棋位，若無同點棋則改移最接近該數的己棋之一。
* 當棋子移到的位置上有任何方棋子，則將該棋子移除棋盤不再使用。
* 以先抵達敵方角落位，或消滅所有敵棋為勝。

---

## 學期專題日誌
105/10 ~ 106/1

----

### 學期專題日誌
#### 例行時間

* 每兩週的週四與助教討論
* 週四早上10~12討論進度
* 週六週日早上9~12共同作業

----

### 學期專題日誌
#### 主要進度內容

* 第4~5週：完成遊戲架構
* 第6~7週：用Minimax實作AI，並把AI與遊戲連接
* 第 8 週：評估盤面、計算Heuristic值
* 第 9 週：期中考週

----

### 學期專題日誌
#### 主要進度內容

* 第10~11週：增加chance node，更新評估盤面的方式，新增比賽用的介面
* 第 12 週：檢討、整理程式碼
* 第13~15週：改用Minimax+Simulation實作，更改所有變數名稱
* 第 16 週：討論random的方式，修改Simulation
* 第17~18週：準備期末、期末考週

----

### 學期專題日誌
#### 比賽 TAAI 2016

* 日期：11/26
* 名次：第四名
* 當時進度：Minimax(4層) + Heuristic evaluation

----

### 學期專題日誌
#### 比賽 TAAI 2016

* 收穫
    * 比賽前需要充足的準備
    * 有圖形化介面在比賽時使用，比較不容易出錯
    * alpha-beta search若有將feature設好，效果可能比Monte-Carlo好
    * alpha-beta search也可不取feature，用類似Monte-Carlo的simulation做

---

## 實作方法

* Minimax chance tree
    * evaluating with feature
    * evaluating with simulation

----

### 實作方法
#### Minimax Chance Tree

* 以Minimax的結構為基礎
* 因為骰子的原故，在衍生出Children時，6顆棋都需要考量(6 children)，因為都有機會被指定使用
* 因為規則的原故，不在場面上的棋子，分數會被選擇為最接近其編號之棋的分數
* 根據不同作法有不同evaluate的方式

----

### 實作方法
#### Evaluating with Feature

* 在Minimax的底層使用我們自訂義的衡量盤面方式
* 依據在棋盤上的位置決定棋子分數，距離目的地越近分數越高
* 不在場面上的棋子，分數會被選擇為最接近其編號之棋的分數
* 將 (我方棋子總合) - (敵方棋子總合 * 策略參數) 當作當前盤面的評量分數回傳
* 策略參數可以影響防守為重或是進攻為重

----

### 實作方法
#### Evaluating with Simulation

* 在Minimax的底層使用模擬來衡量盤面
* 用隨機的方使模擬至終盤，固定次數；勝場為1，敗場為0，總合當作當前盤面的評量分數回傳

----

### 實作方法
#### Evaluating with Simulation

* 隨機-1
    * 根據骰子決定可以移動的棋子
    * 將每顆可移動棋子的合法移動放入array中
    * rand()取出一步(棋子,方向)當作本回合的移動
    * 每一步(棋子,方向)被選擇的機會相等

----

### 實作方法
#### Evaluating with Simulation

* 隨機-1
![](https://i.imgur.com/iUrlVDd.png =700x350)

----

### 實作方法
#### Evaluating with Simulation

* 隨機-2
    * 先rand()出骰子，得出可以走的步(方向)
    * 再rand()這些步確定要怎麼走

* 隨機-3
    * 先rand()出棋子，直到確認該棋子存在
    * 再rand()該旗子的移動方向，直到確認他可以移動

---

## 下學期的可能規劃

* alpha-beta的剪枝
* Bitmap
* 蒙地卡羅
* learning

---

## Github 紀錄
* repo: https://github.com/dream040102/Ewn
* ref: https://github.com/dream040102/Ewn/tree/master/ref

