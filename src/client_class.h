#pragma once

enum _class_id {
	CAI_BaseNPC,
	CAssaultRifle,
	CAutoShotgun,
	CBaseAbility,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAutoShotgun,
	CBaseBackpackItem,
	CBaseBeltItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity,
	CBaseFlex,
	CBaseGrenade,
	CBaseMountedWeapon,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseRifle,
	CBaseShotgun,
	CBaseSniperRifle,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseUpgradeItem,
	CBaseViewModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBoomerClaw,
	CBreakable,
	CBreakableProp,
	CBreakableSurface,
	CButtonTimed,
	CChainsaw,
	CCharge,
	CChargerClaw,
	CClaw,
	CClientPath,
	CColaBottles,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSRagdoll,
	CCSTeam,
	CDynamicLight,
	CDynamicProp,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityParticleTrail,
	CEnvDetailController,
	CEnvDOFController,
	CEnvParticleScript,
	CEnvPhysicsBlocker,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWeaponFire,
	CEnvWind,
	CFinaleTrigger,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFireworkCrate,
	CFirstAidKit,
	CFish,
	CFogController,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncElevator,
	CFuncLadder,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncPlayerGhostInfectedClip,
	CFuncPlayerInfectedClip,
	CFuncRagdollFader,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSimpleLadder,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CGasCan,
	CGnome,
	CGrenadeLauncher,
	CGrenadeLauncher_Projectile,
	CHandleTest,
	Charger,
	CHunterClaw,
	CInferno,
	CInfoLadderDismount,
	CInfoOverlayAccessor,
	CInsectSwarm,
	CItem_Adrenaline,
	CItem_VomitJar,
	CItemAmmoPack,
	CItemBaseUpgradePack,
	CItemDefibrillator,
	CItemUpgradePackExplosive,
	CItemUpgradePackIncendiary,
	CJockeyClaw,
	CLeap,
	CLightGlow,
	CLunge,
	CMagnumPistol,
	CMaterialModifyControl,
	CMolotov,
	CMolotovProjectile,
	COxygenTank,
	CPainPills,
	CParticleFire,
	CParticlePerformanceMonitor,
	CParticleSystem,
	CPhysBox,
	CPhysBoxMultiplayer,
	CPhysicsProp,
	CPhysMagnet,
	CPipeBomb,
	CPipeBombProjectile,
	CPistol,
	CPlasma,
	CPlayerResource,
	CPointCommentaryNode,
	CPointPropUseTarget,
	CPointScriptUseTarget,
	CPoseController,
	CPostProcessController,
	CPrecipitation,
	CPrecipitationBlocker,
	CPredictedViewModel,
	CPropaneTank,
	CPropDoorRotatingCheckpoint,
	CPropHealthCabinet,
	CPropMinigun,
	CPropMountedGun,
	CPropVehicleDriveable,
	CPumpShotgun,
	CRagdollManager,
	CRagdollProp,
	CRagdollPropAttached,
	CRifle_AK47,
	CRifle_Desert,
	CRifle_M60,
	CRifle_SG552,
	CRopeKeyframe,
	CScavengeProgressDisplay,
	CSceneEntity,
	CScriptBaseButton,
	CSelfDestruct,
	CShadowControl,
	CShotgun_Chrome,
	CShotgun_SPAS,
	CSlideshowDisplay,
	CSMG_MP5,
	CSMG_Silenced,
	CSmokerClaw,
	CSmokeStack,
	CSniper_AWP,
	CSniper_Military,
	CSniper_Scout,
	CSniperRifle,
	CSoundMixLayer,
	CSpitAbility,
	CSpitterClaw,
	CSpitterProjectile,
	CSpotlightEnd,
	CSprite,
	CSpriteOriented,
	CSpriteTrail,
	CSteamJet,
	CSubMachinegun,
	CSun,
	CSurvivorDeathModel,
	CSurvivorPosition,
	CSurvivorRescue,
	CTankClaw,
	CTeam,
	CTeamplayRoundBasedRulesProxy,
	CTEArmorRicochet,
	CTEBaseBeam,
	CTEBeamEntPoint,
	CTEBeamEnts,
	CTEBeamFollow,
	CTEBeamLaser,
	CTEBeamPoints,
	CTEBeamRing,
	CTEBeamRingPoint,
	CTEBeamSpline,
	CTEBloodSprite,
	CTEBloodStream,
	CTEBreakModel,
	CTEBSPDecal,
	CTEBubbles,
	CTEBubbleTrail,
	CTEBullets,
	CTEClientProjectile,
	CTEDecal,
	CTEDust,
	CTEDynamicLight,
	CTEEffectDispatch,
	CTEEnergySplash,
	CTEExplosion,
	CTEFizz,
	CTEFootprintDecal,
	CTEFoundryHelpers,
	CTEGaussExplosion,
	CTEGlowSprite,
	CTEImpact,
	CTEKillPlayerAttachments,
	CTELargeFunnel,
	CTEMetalSparks,
	CTEMuzzleFlash,
	CTEParticleSystem,
	CTEPhysicsProp,
	CTEPlayerAnimEvent,
	CTEPlayerDecal,
	CTEProjectedDecal,
	CTerrorGameRulesProxy,
	CTerrorGun,
	CTerrorMeleeWeapon,
	CTerrorPlayer,
	CTerrorPlayerResource,
	CTerrorViewModel,
	CTerrorWeapon,
	CTEShatterSurface,
	CTEShowLine,
	CTESmoke,
	CTESparks,
	CTESprite,
	CTESpriteSpray,
	CTest_ProxyToggle_Networkable,
	CTestTraceline,
	CTEWorldDecal,
	CThrow,
	CTongue,
	CTransitioningPlayer,
	CTriggerPlayerMovement,
	CTutorLesson,
	CVGuiScreen,
	CVomit,
	CVomitJarProjectile,
	CVoteController,
	CWaterBullet,
	CWaterLODControl,
	CWeaponAmmoSpawn,
	CWeaponCSBase,
	CWeaponCSBaseGun,
	CWeaponScavengeItemSpawn,
	CWeaponSpawn,
	CWorld,
	DustTrail,
	Hunter,
	Infected,
	Jockey,
	MovieExplosion,
	NextBotCombatCharacter,
	ParticleSmokeGrenade,
	RocketTrail,
	Smoker,
	SmokeTrail,
	Spitter,
	SporeExplosion,
	SporeTrail,
	SurvivorBot,
	Tank,
	Witch,
	Boomer = 0,
};

using create_client_class_fn = void*(*)(int ent_num, int serial_num);
using create_event_fn = void*(*)();

struct recv_proxy_data_t;
struct recv_table_t;
struct recv_prop_t;

struct dvariant_t
{
	union
	{
		float m_float;
		long m_int;
		char* m_string;
		void* m_data;
		float m_vec[3];
		__int64 m_int64;
	};

	int m_type;
};

struct recv_proxy_data_t
{
	const recv_prop_t* m_recv_prop;
	dvariant_t m_value;

	int m_element;
	int m_object_id;
};

struct recv_table_t
{
	recv_prop_t* m_props;
	int m_props_num;

	void* m_decoder;
	char* m_net_table_name;

	bool m_initialized;
	bool m_in_main_list;
};

struct recv_prop_t
{
	char* m_var_name;

	int m_recv_type;
	int m_flags;
	int m_string_buffer_sz;

	bool m_inside_array;

	const void* m_extra_data;
	recv_prop_t* m_array_prop;
	void* m_array_length_proxy;
	void* m_proxy_fn;
	void* m_data_table_proxy_fn;
	recv_table_t* m_data_table;

	int m_offset;
	int m_element_stride;
	int m_elements;

	const char* m_parent_array_prop_name;
};

struct client_class_t {
	create_client_class_fn* m_create_fn;
	create_event_fn* m_create_event_fn;

	const char* m_network_name;

	recv_prop_t* m_recv_table;
	client_class_t* m_next;

	_class_id m_class_id;
};