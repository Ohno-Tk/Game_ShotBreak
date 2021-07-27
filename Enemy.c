/*=============================================================================

		敵[ Enemy.cpp ]

-------------------------------------------------------------------------------
	■　作成日
		2016/01/05
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <time.h>
#include <math.h>
#include "main.h"
#include "Enemy.h"
#include "Shadow.h"
#include "fade.h"
#include "MeshWall.h"
#include "Building.h"
#include "Utility.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define MODEL_NAME "data/MODEL/Enemy/Robo.x"

#define MAX_MAT ( 4 )	//	モデルの最大マテリアル数

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	ターゲットのセット
void SetEnemy( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point );

//	当たり判定
void CollisionEnemy( int Cnt );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPD3DXMESH g_pMeshEnemy;	//	メッシュインターフェースポインタ

LPD3DXBUFFER g_pBufferMatEnemy;	//	マテリアル情報

DWORD g_nNumMatEnemy;	//	マテリアル数

LPDIRECT3DTEXTURE9 g_pTextureEnemy[ MAX_MAT ] = { NULL };//	テクスチャインターフェース

D3DXMATERIAL* pMatEnemy;	//	マテリアルポインタ

ENEMY g_Enemy[ MAX_ENEMY ];	//	敵構造体

/*-----------------------------------------------------------------------------
 関数名:	void InitTarget( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitEnemy( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	次のモードの取得
	MODE *Mode = GetNextMode();

	//	現在の時刻を取得
	srand((unsigned)time(NULL));


	HRESULT hr;

	hr = D3DXLoadMeshFromX( MODEL_NAME ,			//	ファイル名
							D3DXMESH_MANAGED,
							pDevice,				//	デバイス
							NULL,					//	隣接バッファ
							&g_pBufferMatEnemy,		//	マテリアル情報を格納
							NULL,
							&g_nNumMatEnemy,		//	マテリアル数
							&g_pMeshEnemy );		//	メッシュ

	//	モデルのエラーチェック
	if( FAILED( hr ) )
	{

		MessageBox( NULL , "[ Enemy.cpp ]\n MODEL_NAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	マテリアル情報のポインタとしてバッファのアドレスを取得
	pMatEnemy = ( D3DXMATERIAL* )g_pBufferMatEnemy -> GetBufferPointer();

	for( int i = 0 ; i < ( int )g_nNumMatEnemy ; i++ )
	{
		if( pMatEnemy[ i ].pTextureFilename != NULL )
		{

			//	テクスチャのエラーチェック
			if( FAILED( D3DXCreateTextureFromFile( pDevice , pMatEnemy[ i ].pTextureFilename , &g_pTextureEnemy[ i ]  ) ) )
			{

				MessageBox( NULL , "[ Enemy.cpp ]\n MODEL_NAME\nの画像の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

			}	//	end of if

		}	//	end of if

	}	//	end of for


	//	構造体初期化
	for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
	{

		//	座標
		g_Enemy[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	拡大率
		g_Enemy[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	回転量
		g_Enemy[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	前の座標
		g_Enemy[ Cnt ].OldPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	移動量
		g_Enemy[ Cnt ].Move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	半径
		g_Enemy[ Cnt ].Radius = 3.0f;

		//	使用フラ
		g_Enemy[ Cnt ].Use = false;

	}	//	end of for


	//	モードがタイトルの時
	if( *Mode == MODE_TITLE )
	{

		//	ターゲットのセット
		SetEnemy( D3DXVECTOR3( 0.0f , 0.0f , -40.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , ( TYPE_POINT )0 );
		SetEnemy( D3DXVECTOR3( -30.0f , 0.0f , -20.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , ( TYPE_POINT )0 );
		SetEnemy( D3DXVECTOR3( 30.0f , 0.0f , -20.0f ), D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , ( TYPE_POINT )0 );

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitTarget( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitEnemy( void )
{

	if( g_pMeshEnemy != NULL )	//	メッシュインターフェースポインタ
	{

		g_pMeshEnemy -> Release();
		g_pMeshEnemy = NULL;

	}	//	end of if

	if( g_pBufferMatEnemy != NULL )	//	マテリアル情報開放
	{

		g_pBufferMatEnemy -> Release();
		g_pBufferMatEnemy = NULL;

	}	//	end of if


}	//	end of func
 
/*-----------------------------------------------------------------------------
 関数名:	void UpdataTarget( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateEnemy( void )
{

	//	影の情報取得
	SHADOW *Shadow = GetShadow( 0 );

	//	次のモードの取得
	MODE *Mode = GetNextMode();



	if( *Mode == MODE_GAME )
	{

		for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
		{

			if( g_Enemy[ Cnt ].Use == true)
			{

				//	前回の座標を取得
				g_Enemy[ Cnt ].OldPos = g_Enemy[ Cnt ].World.Pos;


				g_Enemy[ Cnt ].World.Pos += g_Enemy[ Cnt ].Move;


				//	当たり判定
				CollisionEnemy( Cnt );

			}	//	end of if

		}	//	end of for



		//	敵の出現位置の決定
		
		int i = rand() % TYPE_MAX;	//	出現位置の決める変数
		TYPE_POINT Point = ( TYPE_POINT )i;


		if( Point == TYPE_POINT1 )
		{

			//	ターゲットのセット
			SetEnemy( D3DXVECTOR3( 1000.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 2.5f , 0.0f , 2.5f ) , Point );

		}	//	end of if

		else if( Point == TYPE_POINT2 )
		{

			//	ターゲットのセット
			SetEnemy( D3DXVECTOR3( -1000.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 2.0f , 0.0f , 2.0f ) , Point );

		}	//	end of else if

		else if( Point == TYPE_POINT3 )
		{

			//	ターゲットのセット
			SetEnemy( D3DXVECTOR3( 0.0f , 0.0f , 1000.0f ) , D3DXVECTOR3( 1.5f , 0.0f , 1.5f ) , Point );

		}	//	end of else if
	

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawTarget( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawEnemy( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();



	//	現在デバイスに設定されているマテリアル情報の取得
	D3DMATERIAL9 matDef;
	pDevice -> GetMaterial( &matDef );


	for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
	{

		if( g_Enemy[ Cnt ].Use == true)
		{

			//	逆行列なしのワールド座標変換
			SetWorld( g_Enemy[ Cnt ].World.Pos , g_Enemy[ Cnt ].World.Rot , g_Enemy[ Cnt ].World.Scl );


			for( int i = 0 ; i < ( int )g_nNumMatEnemy ; i++ )
			{
				//	デバイスにマテリアルを設定
				pDevice -> SetMaterial( &pMatEnemy[ i ].MatD3D );

				//	テクスチャ設定
				pDevice -> SetTexture( 0 , g_pTextureEnemy[ i ] );


				//	描画
				g_pMeshEnemy -> DrawSubset( i );

			}	//	end of for

		}	//	end of if

		pDevice -> SetMaterial( &matDef );

	}


}	//	end of func


/*-----------------------------------------------------------------------------
 関数名:	void SetTarget( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Move	移動量
			TYPE_POINT Point	出現位置
 戻り値:	
 説明:		ターゲットのセット
-----------------------------------------------------------------------------*/
void SetEnemy( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point )
{

	for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
	{

		if( g_Enemy[ Cnt ].Use == false )
		{

			//	座標
			g_Enemy[ Cnt ].World.Pos = Pos;

			//	移動量
			g_Enemy[ Cnt ].Move = Move;

			//	出現位置
			g_Enemy[ Cnt ].Point = Point;

			//	使用フラグ
			g_Enemy[ Cnt ].Use = true;

			//	影のセット
//			SetShadow( g_Enemy[ Cnt ].World.Pos , 15.0f , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

			break;

		}	//	end of if
		
	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void CollisionEnemy( int Cnt )
 引数:		int Cnt		敵構造体のカウンタ
 戻り値:	
 説明:		当たり判定
-----------------------------------------------------------------------------*/
void CollisionEnemy( int Cnt )
{

	//	メッシュウォール情報取得
	MESHWALL *MeshWall = GetMeshWall();


	//	壁との判定
	if( g_Enemy[ Cnt ].World.Pos.z + 10.0f > MeshWall[ 0 ].World.Pos.z )
	{

		g_Enemy[ Cnt ].Move.z *= -1.0f;

	}	//	end of if

	else if( g_Enemy[ Cnt ].World.Pos.z - 10.0f < MeshWall[ 1 ].World.Pos.z )
	{

		g_Enemy[ Cnt ].Move.z *= -1.0f;

	}	//	end of else if

	else if( g_Enemy[ Cnt ].World.Pos.x - 10.0f < MeshWall[ 3 ].World.Pos.x )
	{

		g_Enemy[ Cnt ].Move.x *= -1.0f;

	}	//	end of else if

	else if( g_Enemy[ Cnt ].World.Pos.x + 10.0f > MeshWall[ 2 ].World.Pos.x )
	{

		g_Enemy[ Cnt ].Move.x *= -1.0f;

	}	//	end of else if



	//	建物情報の取得
	BUILDING *Building = GetBuilding();

	for( int CntBuilding = 0 ; CntBuilding < MAX_BUILDING ; CntBuilding++ , Building++ )
	{
		//	球と球の当たり判定
		if( SphereCollision( D3DXVECTOR3( g_Enemy[ Cnt ].World.Pos.x , g_Enemy[ Cnt ].World.Pos.y + 10.0f , g_Enemy[ Cnt ].World.Pos.z ) , D3DXVECTOR3( Building -> World.Pos.x , Building -> World.Pos.y + 30.0f , Building -> World.Pos.z ) , g_Enemy[ Cnt ].Radius , Building -> Radius ) )
		{

			if( g_Enemy[ Cnt ].Point = TYPE_POINT1 )
			{

				g_Enemy[ Cnt ].Move.x *= -1.0f;

			}	//	end of if

			else if( g_Enemy[ Cnt ].Point = TYPE_POINT2 )
			{

				g_Enemy[ Cnt ].Move.x *= -1.0f;

			}	//	end of else if

			else if( g_Enemy[ Cnt ].Point = TYPE_POINT2 )
			{

				g_Enemy[ Cnt ].Move.z *= -1.0f;

			}	//	end of else if

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	ENEMY *GetEnemyNor( void )
 引数:		
 戻り値:	
 説明:		ターゲット情報の取得
-----------------------------------------------------------------------------*/
ENEMY *GetEnemyNor( void )
{

	return &g_Enemy[ 0 ];

}	//	end of func
