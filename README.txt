2024/05/28
auther: Shunsuke.I

保存したlogファイルをnotionで管理するために作成したプログラム

①　log2notion.cpp : logが入っているディレクトリ名をnotionのデータベースに保存する
例)
log/
 |
 |-2024.0329.1150/
 |  |-imu.log
 |  |-rtk_gnss.log
 |
 |-2024.0328.1220/
 |  |-imu.log
 |  |-rtk_gnss.log

Notionの特定のデータベースに下記の名前でページが作成される。
-->2024.0329.1150/
-->2024.0328.1220/

********実行方法*********
１．notionAPIトークンを取得

２．データベースIDを取得

３．コンパイル　
    mkdir bin
    cmake ..
    make

４．実行
    ./log2notion -p {logの入っているディレクトリを指定（上記の例では:log/）}


***notionAPIでは画像を埋め込む機能がないため、自動で作成した図をデータベース内に埋め込むことができなかった。***

②　auto_plotgnss.cpp: logのディレクトリ名を自動で取得、直交座標系のxyzのプロットをする。
１．plot_gnss/とlog2notion/の２つのプログラムをコンパイルをかける
２．./auto_plotgnss -p {logのディレクトリが入っているパス(上記のものと一緒)}

*保存は、各logディレクトリ内に保存される



