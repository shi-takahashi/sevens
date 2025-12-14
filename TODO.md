# Sevens アプリ リリース準備タスク

## 📅 作成日: 2024年12月14日

---

## ✅ 完了したタスク（2024/12/14）

### ビルド環境の現代化
- [x] cocos2d-x 3.17.2のダウンロードと統合
- [x] Gradle 8.5 + Android Gradle Plugin 8.1.4へアップデート
- [x] jcenter → mavenCentral への移行
- [x] targetSdkVersion 34（Android 14対応）
- [x] 64ビット対応（armeabi-v7a + arm64-v8a）
- [x] AndroidX対応
- [x] CMakeビルドシステムへ切り替え（ndk-buildから）

### 依存関係の更新
- [x] AdMob SDK 19.1.0 → 22.6.0への更新
- [x] 新しいAdMob APIへの対応（InterstitialAd.load()）
- [x] OPPO最適化コードの削除（廃止されたAPI）

### Androidマニフェストの修正
- [x] android:exported="true" の追加（Android 12+対応）
- [x] allowBackup="false" の設定（セキュリティ対策）
- [x] namespace の追加（AGP 8.x対応）

### ビルド成果物
- [x] デバッグAPK生成（動作確認済み）
- [x] リリースAAB生成（署名済み）
  - パス: `proj.android/app/build/outputs/bundle/release/Sevens-release.aab`
  - サイズ: 20MB
  - 署名: keystore（2044年まで有効）

---

## 🚀 次のステップ

### 優先度：高（Google Play公開前に必須）

#### 1. プライバシーポリシーの作成
**理由**: AdMobを使用しているため必須

**タスク**:
- [ ] プライバシーポリシーページを作成（HTML）
- [ ] Webサーバーにアップロード（例: GitHub Pages、個人サーバー）
- [ ] URLを取得
- [ ] Google Play Consoleに登録

**プライバシーポリシーに含めるべき内容**:
- 収集する情報（広告ID、デバイス情報）
- AdMobの使用について
- データの利用目的
- 第三者との共有（Google AdMob）
- 連絡先情報

**参考テンプレート**:
```
https://www.privacypolicies.com/blog/privacy-policy-template/
```

#### 2. Google Play Consoleの設定
**必要な作業**:
- [ ] データセーフティの申告
  - 広告IDの収集を申告
  - 収集データの用途を説明
- [ ] アプリコンテンツの設定
  - 広告の有無: あり
  - ターゲット年齢層を設定
  - コンテンツレーティング
- [ ] プライバシーポリシーURLの登録

---

### 優先度：中（機能改善）

#### 3. ランキング機能のFirestore移行
**現状**:
- PHP + MySQL（`https://x723.xsrv.jp/sevens/get_ranking.php`）
- データベースが存在しない

**移行方法の選択肢**:

**オプションA: Firebase Firestore + REST API**
- Firebase Firestoreでランキングデータを管理
- cocos2d-xからHTTP経由でアクセス
- 実装難易度: 中
- メリット: C++コード変更が少ない

**オプションB: Firebase C++ SDK**
- Firebase C++ SDKを直接統合
- 実装難易度: 高
- メリット: リアルタイム更新が可能

**タスク**:
- [ ] Firebaseプロジェクトの作成
- [ ] Firestoreデータベースの設計
  ```
  rankings/
    {userId}/
      - name: string
      - score: number
      - timestamp: timestamp
  ```
- [ ] セキュリティルールの設定
- [ ] 実装方法の選択
- [ ] コード実装
- [ ] テスト

**関連ファイル**:
- `Classes/NetRanking.cpp`
- `Classes/NetRanking.h`

#### 4. アプリ終了ボタンの追加
**実装方法**:
- UIにボタンを追加
- タップ時の処理:
  ```cpp
  Director::getInstance()->end();
  exit(0);
  ```

**タスク**:
- [ ] ボタン画像の準備
- [ ] TitleSceneまたはOptionDialogに配置
- [ ] イベントハンドラの実装
- [ ] テスト

**関連ファイル**:
- `Classes/TitleScene.cpp`
- `Classes/OptionDialog.cpp`

#### 5. UI/見た目の改善
**検討事項**:
- [ ] 現在のUIの評価
- [ ] 改善ポイントの洗い出し
- [ ] デザイン案の作成
- [ ] リソース（画像、フォント）の準備
- [ ] 実装

**リソースパス**:
- `Resources/` ディレクトリ

---

### 優先度：低（将来的な改善）

#### 6. Axmol Engineへの移行
**理由**:
- cocos2d-x 3.17.2は古い（2019年リリース）
- Axmolは活発に開発されている後継エンジン

**メリット**:
- 最新のAndroid/iOS対応
- バグ修正とセキュリティアップデート
- コミュニティサポート

**デメリット**:
- コード修正が必要
- テストに時間がかかる

**タスク**:
- [ ] Axmol Engineのドキュメント確認
- [ ] 移行ガイドの確認
- [ ] 移行の工数見積もり
- [ ] 実装（別ブランチで作業推奨）

---

## 📋 Google Play公開チェックリスト

### アプリの準備
- [x] AABファイルの生成
- [x] 署名の確認
- [x] 64ビット対応
- [x] targetSdkVersion 34
- [ ] プライバシーポリシーURL

### Google Play Consoleでの作業
- [ ] アプリの作成/選択
- [ ] AABのアップロード
- [ ] ストアリスティングの作成
  - アプリ名
  - 簡単な説明
  - 詳しい説明
  - スクリーンショット（最低2枚）
  - アイコン
- [ ] データセーフティの申告
- [ ] プライバシーポリシーの登録
- [ ] コンテンツレーティングの取得
- [ ] 価格と配信地域の設定
- [ ] リリースノートの作成
- [ ] 審査に送信

---

## 📂 重要なファイルとパス

### ビルド成果物
- AABファイル: `proj.android/app/build/outputs/bundle/release/Sevens-release.aab`
- APKファイル: `proj.android/app/build/outputs/apk/debug/Sevens-debug.apk`

### 設定ファイル
- Gradle設定: `proj.android/gradle.properties`
- ビルド設定: `proj.android/app/build.gradle`
- マニフェスト: `proj.android/app/AndroidManifest.xml`
- 署名鍵: `proj.android/keystore`

### ソースコード
- ゲームロジック: `Classes/`
- リソース: `Resources/`
- Android JNI: `proj.android/app/jni/`
- CMake設定: `CMakeLists.txt`

---

## 🔧 ビルドコマンド

### デバッグビルド
```bash
cd /Users/takahashi-sh/Project/Sevens-master/proj.android
./gradlew assembleDebug
```

### リリースAABビルド
```bash
cd /Users/takahashi-sh/Project/Sevens-master/proj.android
./gradlew bundleRelease
```

### クリーンビルド
```bash
./gradlew clean assembleDebug
```

---

## 📞 サポート情報

### 署名情報
- Keystore: `proj.android/keystore`
- Alias: `mykey`
- パスワード: （gradle.propertiesに設定済み）
- 有効期限: 2044年3月30日

### AdMob情報
- App ID: `ca-app-pub-4630894580841955~3122250629`
- 広告ユニットID: `ca-app-pub-4630894580841955/8749981826`

### パッケージ情報
- Package Name: `s_takahashi.java_conf.gr.jp`
- Version Code: 14
- Version Name: 1.14

---

## 💡 参考リンク

- [Google Play Console](https://play.google.com/console)
- [Android Developers - App Bundle](https://developer.android.com/guide/app-bundle)
- [AdMob ポリシー](https://support.google.com/admob/answer/6128543)
- [Firebase Console](https://console.firebase.google.com/)
- [cocos2d-x Documentation](https://docs.cocos2d-x.org/cocos2d-x/v3/en/)

---

## 📝 メモ

### 既知の警告（問題なし）
- NDK pathの警告: 動作には影響なし
- 非推奨APIの警告: cocos2d-x 3.17.2の仕様
- Gradle 9.0互換性警告: 現在は問題なし

### 次回のセッション開始時
1. このTODO.mdを確認
2. 優先度の高いタスクから着手
3. 完了したタスクにチェックを入れる

---

**最終更新**: 2024年12月14日
**次回作業予定**: プライバシーポリシーの作成 → Google Play Console設定
