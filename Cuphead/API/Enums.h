#pragma once

enum class ESceneType
{
	NONE,
	Title,
	MainMenu,
	PlayMap,
	Tutorial,
    MoveWorldIntro,
	InWorldIntro,
	SaltBakeryShop,
	SaltBakerBossIntro,
	SaltBakerBoss,
	Ending,
	MAX,
};
enum class  ELayerType
{
	NONE,
	BackObject,
	BackColObject,
	Boss,
	MiddleObject,
	Ground,
	Monster,
	Player,
	Bullet,
	PlayerBullet,
	FrontObject,
	Effect,
	UI,
	MAX,
};
enum class EGameObjectType
{
	NONE,
	Player,
	Boss,
	MAX,
};
enum class EComponentType
{
	NONE,
	Transform,
	SpriteRenderer,
	Animator,
	Collider,
	Audio,
	Rigidbody,
	Script,
	MAX,
};
enum class ERenderType
{
	BitBlt,
	TransParentBlt,
	StretchBlt,
};
enum class EPlayerType
{
	Cuphead,
	MugMan,
	MsChalice,
	MAX
};
enum class EWindowType
{
	MainMenu,
	Options,
	MAX

};
enum class EDirType
{
	STAY=1,
	LEFT=2,
	RIGHT=4,
	UP=8,
	DOWN=16,
	SIT=32,
	RUN=64,
	MAX
};
enum class EStateType
{
	START,
	ING,
	NOTHING,
};
enum class EShotBulletType
{
	NONE,
	Direct,
	Guided_Missile,
	ZigZag,
	Scatter,
	MAX
};
enum class EPhaseType
{
	NONE,
	PHASE1,
	PHASE2,
	PHASE3,
	MAX
};