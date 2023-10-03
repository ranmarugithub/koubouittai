# include <Siv3D.hpp>

//シーンの名前
enum class State
{
	Title,
	Game,
};

using App = SceneManager<String>;

// タイトルシーン

class Title : public App::Scene
{
public:

	// コンストラクタ（必ず実装）
	Title(const InitData& init)
		: IScene{ init }
	{

	}

	// 更新関数（オプション）
	void update() override
	{
		if (MouseL.down())
		{
			changeScene(U"Game");
		}
	}

	// 描画関数（オプション）
	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });

		FontAsset(U"TitleFont")(U"My Game").drawAt(400, 100);

		Circle{ Cursor::Pos(), 50 }.draw(Palette::Orange);
	}
};

class Game : public App::Scene
{
public:

	// コンストラクタ（必ず実装）
	Game(const InitData& init)
		: IScene{ init }
	{

	}

	// 更新関数（オプション）
	void update() override
	{
		Circle circle;

		double directionx = 0.0;
		double directiony = 0.0;
		bool shotenable = false;

		Vec2 pos{ 400, 300 };

		const double speed = 200;

		while (System::Update())
		{
			ClearPrint();

			const double deltaTime = Scene::DeltaTime();
			circle = Circle{ pos,20 };


			//弾を発射
			if (KeySpace.down())
			{
				shotenable = true;
			}

			//弾の移動入力
			// ←
			if (KeyA.pressed())
			{
				if (directionx > -1.0)
				{
					directionx -= 0.01;
				}

				if (directiony > 0)
				{
					directiony -= 0.005;
				}
				if (directiony < 0)
				{
					directiony += 0.005;
				}
			}

			// → 
			if (KeyD.pressed())
			{
				if (directionx < 1.0)
				{
					directionx += 0.01;
				}

				if (directiony > 0)
				{
					directiony -= 0.005;
				}
				if (directiony < 0)
				{
					directiony += 0.005;
				}
			}

			// ↑ 
			if (KeyW.pressed())
			{
				if (directiony > -1.0)
				{
					directiony -= 0.01;
				}

				if (directionx > 0)
				{
					directionx -= 0.005;
				}
				if (directionx < 0)
				{
					directionx += 0.005;
				}
			}

			// ↓ 
			if (KeyS.pressed())
			{
				if (directiony < 1.0)
				{
					directiony += 0.01;
				}

				if (directionx > 0)
				{
					directionx -= 0.005;
				}
				if (directionx < 0)
				{
					directionx += 0.005;
				}
			}

			if (shotenable == true)
			{
				//弾の移動
				pos.x += (speed * deltaTime * directionx);
				pos.y += (speed * deltaTime * directiony);

				//弾を無効にする
				if (pos.x < 0 || pos.x > 800 || pos.y < 0 || pos.y > 600)
				{
					shotenable = false;
					pos = { 400, 300 };
				}
			}

			//円を描く
			circle.draw(Palette::White);

			//デバッグ
			Print << U"{:.4f}"_fmt(pos);
			Print << U"{:.4f}"_fmt(directionx);
			Print << U"{:.4f}"_fmt(directiony);
			Print << shotenable;
		}
	}

	// 描画関数（オプション）
	void draw() const override
	{

	}
};

void Main()
{
	FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);

	// シーンマネージャーを作成
	App manager;

	// タイトルシーン（名前は "Title"）を登録
	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");

	while (System::Update())
	{
		// 現在のシーンを実行
		// シーンに実装した .update() と .draw() が実行される
		if (not manager.update())
		{
			break;
		}
	}
}


