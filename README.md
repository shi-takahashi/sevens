# Sevens

七並べゲーム（cocos2d-x 3.17.2）

## ディレクトリ構成

```
sevens/
├── Classes/           # C++ソースコード
│   ├── AppDelegate.*  # アプリケーションエントリポイント
│   ├── GameScene.*    # ゲーム画面
│   ├── Card.*         # カードクラス
│   ├── Character.*    # キャラクタークラス
│   ├── AI*.*/         # AIロジック
│   └── ...
│
├── Resources/         # リソースファイル（画像・音声等）
│   ├── card/          # カード画像
│   ├── fonts/         # フォントファイル
│   ├── game/          # ゲーム画面用リソース
│   ├── sound/         # 効果音・BGM
│   └── title/         # タイトル画面用リソース
│
├── CMakeLists.txt     # CMakeビルド設定
│
├── cocos2d/           # cocos2d-x本体（別途配置が必要）
│
├── proj.android/      # Androidプロジェクト
└── proj.ios_mac/      # iOS/macOSプロジェクト
```

## セットアップ

1. 最新Androidに対応したcocos2d本体を(ドライブに保存) を `cocos2d/` ディレクトリに配置
2. Android Studio で proj.android を開く
