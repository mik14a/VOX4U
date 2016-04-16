# VOX4U
[MagicaVoxel](https://voxel.codeplex.com) の VOX データを
[Unreal Engine 4](https://www.unrealengine.com/) にインポートするためのプラグインです．

## 動作環境

Unreal Engine 4.10.4以降

## 使い方

VOX ファイルを UE4 にドラッグ＆ドロップするか，インポートダイアログから VOX ファイルをインポートしてください．

現在次の形式のインポートに対応しています．
* StaticMesh
* DestructibleMesh
* Voxel

### インポートオプション

インポートダイアログのインポート形式ごとの切り替えなどの部分がおかしいですが，
インポート時に設定を行うことが出来ます．

#### StaticMesh

* ImportXForward
  * インポート時 MagicaVoxel の Y 軸を UE4 での X 軸に変更します．
* ImportXYCenter
  * XY 平面上での中心にデータをインポートします．
* BuildSettings
  * メッシュをビルドする設定です．デフォルトではスケールが10倍，UseMikkTSpace が false になっています．
* MergeVertexes
  * 共有頂点データを作成します．
* Material
  * マテリアルを指定します．MagicaVoxel からエクスポートしたパレットを使用します．設定されていなければ自動で作成されます．

### DestructibleMesh
* ImportXForward
  * インポート時 MagicaVoxel の Y 軸を UE4 での X 軸に変更します．
* ImportXYCenter
  * XY 平面上での中心にデータをインポートします．
* BuildSettings
  * メッシュをビルドする設定です．デフォルトではスケールが10倍，UseMikkTSpace が false になっています．
* Material
  * マテリアルを指定します．MagicaVoxel からエクスポートしたパレットを使用します．設定されていなければ自動で作成されます．

### Voxel
* ImportXForward
  * インポート時 MagicaVoxel の Y 軸を UE4 での X 軸に変更します．
* ImportXYCenter
  * XY 平面上での中心にデータをインポートします．
* Mesh
  * ボクセルを構成するメッシュを指定します．

## インストール

{ProjectName}/Plugins/VOX4U にソースをチェックアウトしてビルド，もしくはバイナリリリースからダウンロードしたファイルを置いてください．

## ライセンス

[MIT License](https://github.com/mik14a/VOX4U/blob/master/LICENSE)
