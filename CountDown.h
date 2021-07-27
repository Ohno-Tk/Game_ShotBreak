/*=============================================================================

		カウントダウン[ CountDown.h ]

-------------------------------------------------------------------------------

	■　作成日
		2016/01/05
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	二重インクルード防止
-----------------------------------------------------------------------------*/
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
void InitCountDown( void );		//	カウントダウンの初期化
void UninitCountDown( void );	//	カウントダウンの終了
void UpdateCountDown( void );	//	カウントダウンの更新
void DrawCountDown( void );		//	カウントダウンの描画

bool *GetStart( void );			//	スタートしたかの情報取得

#endif
