# 《决战李文正之巅》数值策划

## 等级

* 模仿SEU绩点,设定0.0为初始级别 , 4.8为最高级别。经验值按照下列规律

* $$
  \begin{equation}
  exp(n)=
  \left\{
               \begin{array}{lr}
               1,1 & n=0,n=1 \\
               exp(n-1)+exp(n-2)& 2<=n<=12\\
               C*exp(n-1), &  13<=n<=48
               \end{array}
  \right.
  \end{equation}
  $$

* 上式$C$待定

* > 蜘蛛 : $3*u$ 
  >
  > 皮皮虾：$5*u$
  >
  > Boss：$100*u​$

## 血量

### 人物

$HP = (initialHP=100) + Level*100$

### 怪物

$HP=(initialHP+100) + SystemTimeBuffer $

$BossHP = 2000 $

## 伤害

### 人物

$Damage = (initial Damage = 10) + Level * 10  + EquipmentBuff$

### 怪物

$Damage =(initialDamage = 8) + SystemTimeBuffer $

$Boss = 25$

## 物品



## 速度

* 人物：$Speed = initialSpeed = 0.3 + Level() * 0.1$
* 近战怪物：$Speed = initialSpeed = 0.4 + SystemTimeBuffer$
* 远程怪物：$Speed  = initialSpeed = 0.3 + SystemTimeBuffer$  

## 时间

预计10~15min可结束游戏，每当最大玩家等级>4的倍数时给怪物提升怪物实力，提升方案如下：

> 速度加成: 近战+0.5，远程+0.25
>
> 伤害加成: +2

