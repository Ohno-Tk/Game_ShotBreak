/*=============================================================================

		ライト[ light.h ]

-------------------------------------------------------------------------------

	■　作成日
		2016/10/19
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	二重インクルード防止
-----------------------------------------------------------------------------*/
#ifndef _LIGHT_H_
#define _LIGHT_H_

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
void InitLight( void );		//	ライトの初期化
void UninitLight( void );	//	ライトの終了
void UpdateLight( void );	//	ライトの更新
void DrawLight( void );	//	ライトの描画
void SetLight( void );	//	ライトのセット

#endif
