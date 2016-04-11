# VOX4U
[MagicaVoxel](https://voxel.codeplex.com) の VOX データを
[Unreal Engine 4](https://www.unrealengine.com/) にインポートするためのプラグインです．

## 動作環境

Unreal Engine 4.10.4以降

## 使い方

VOX ファイルを UE4 にドラッグ＆ドロップするか，インポートダイアログから VOX ファイルをインポートしてください．

現在次の形式のインポートに対応しています．
* StaticMesh
* MeshedVoxel(実験)
* InstancedVoxel

### インポートオプション

インポートダイアログのインポート形式ごとの切り替えなどの部分がおかしいですが，
インポート時に設定を行うことが出来ます．

#### StaticMesh

* ImportXForward
  * インポート時 MagicaVoxel の Y 軸を UE4 での X 軸に変更します．
* ImportXYCenter
  * XY 平面上での中心にデータをインポートします．
* BuildSettings
  * メッシュをビルドする設定です．この設定は，後で StaticMesh アセットの詳細から変更可能です．
  デフォルトではスケールが10倍，UseMikkTSpace が false になっています．
* MergeVertexes
  * 共有頂点データを作成します．
* Material
  * マテリアルを指定します．
  MagicaVoxel に同梱されている pal フォルダにある png ファイルををインポートしたデータを使用する事を想定しています．

### MeshedVoxel
* ImportXForward
  * インポート時 UE4 でのフォワードに整えます．
* ImportXYCenter
  * XY平面上での中心にデータをインポートします．

### InstancedVoxel
* ImportXForward
  * インポート時 UE4 でのフォワードに整えます．
* ImportXYCenter
  * XY平面上での中心にデータをインポートします．
* Mesh
  * ボクセルを構成するメッシュを指定します．
  指定されたメッシュが1ボクセルになるようにインスタンス表示されます．

## インストール

{ProjectName}/Plugins/VOX4U にソースをチェックアウトしてビルド，
もしくはバイナリリリースからダウンロードしたファイルを置いてください．

{ProjectName}/Plugins/VOX4U/Content に 1uu サイズのキューブがあります．
Voxel 形式のアセットを使用した場合は Mesh に指定すれば，形状の確認を行えます．

## ライセンス

[MIT License](https://github.com/mik14a/VOX4U/blob/master/LICENSE)
