# sony_hack

sony のハッカソンで作った，自動眼鏡着脱装置「gOwOgL」の開発メモ  
12/8 作成  
<sonyhack_master.ino>  
マスターに使うプログラム．モーターへのプログラム等を載せている．  
<sonyhack_slave_cam.ino>  
スレーブ側のスプレセンスに用いる．主に顔認証  
<dataset2.ino>  
顔認証の機械学習に用いるデータセットを取るためのプログラム．

2021/12/09 更新
<LINE Notify.ino>
ESP32を用いてWi-Fi通信
→LINE Notifyにアクセス
→通知の送信
