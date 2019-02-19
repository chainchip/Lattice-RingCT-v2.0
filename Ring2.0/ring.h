#ifndef RING_H
#define RING_H


#include "params.h"
#include "poly.h"

/**
*function: setup

*/

typedef struct {
	poly a;
	poly cn;
} IW;

void LRCT_Setup(poly *A, poly *H, size_t mLen);
void LRCT_KeyGen(poly *a, poly *A, poly *S, size_t mLen);
void LRCT_SigGen(poly *c1, poly **t, poly *h, poly *A, poly *H,
				 poly *S, poly *u, size_t mLen, poly *L, uint8_t w,
				 uint8_t pai, unsigned char *msg, int msgLen);
int LRCT_SigVer(const poly *c1, poly **t, poly *A, poly *H, size_t mLen, poly *h, poly *L,
				uint8_t w, unsigned char *msg, int msgLen);
/////单个输出交易方案
/*
方案说明：
*/
void LRCT_Mint(IW *iw, poly *ck, poly *a, poly *A, size_t mLen, unsigned char* bMessage, size_t msglen);
void LRCT_Spend(IW *iwOA, poly *ckOA, poly *c1, poly **t, poly *h, poly *L, unsigned char* bSignMess, size_t sigMsgLen, IW *iws, size_t iwsLen,
				int PaiInd, poly *skPai, poly *ckPai, unsigned char* bVal, size_t bvalLen, poly *OA, poly *A, poly *H, size_t mLen);
int LRCT_Verify(poly *c1, poly **t, poly *h, poly* A, poly *H, size_t mLen, 
	unsigned char* bSignMess, size_t sigMsgLen, poly *L, size_t iwsLen);

//
/*
多输入多输出签名方案
*/

/*
函数声明：系统初始化，产生公共参数
输出：公共矩阵A,H,行数mLen
*/
void MIMO_LRCT_Setup(poly *A, poly *H, size_t mLen);
/*
函数声明：密钥产生。
输入：矩阵A,私钥S, 矩阵行数mLen
输出：用户公钥
*/
void MIMO_LRCT_KeyGen(poly *a, poly *A, poly *S, size_t mLen);
/*
函数声明：签名产生
输入：私钥列表SList, 长度NLen, 公开参数A, H, 矩阵宽度mLen, 公钥列表LList, 长度wLen, 交易者位置pai, 签名消息msg,消息长度msgLen
输出：多项式c1, t列表tList, h列表hList.
*/
void MIMO_LRCT_SigGen(poly *c1, poly *tList, poly *hList, poly *SList, int NLen,
	poly *A, poly *H, int mLen, poly *LList, int wLen, uint8_t pai, unsigned char *msg, int msgLen);
/*
函数声明：签名验证
输入：签名（c1， t列表tList, h列表：hList, 交易个数，公共参数A,H, 矩阵宽度 mLen, 公钥列表LList, 列表长度wLen, 签名消息，消息长度）
*/
int MIMO_LRCT_SigVer(poly *c1, poly *tList, poly *hList, int NLen, poly *A, poly *H,
	size_t mLen, poly *LList, int wLen, unsigned char *msg, int msgLen);
/*

*/
void MIMO_LRCT_Mint(IW *iw, poly *ck, poly *a, poly *A, size_t mLen, unsigned char* bMessage, size_t msglen);
/// 
void MIMO_LRCT_Hash(int *pTable, poly *cn, poly *a, poly *ia, int beta);

//////
void ZKP_OR(poly *ck , int bit, int betaLen);
////////////

void LRCT_Lift(poly *LA, poly *A, poly *a, size_t mLen);
/*
函数声明：承诺消息m， r = A*sk + m 
输入：公共矩阵A, 私钥sk, 矩阵行数 mLen,  承认消息m,消息长度bMessage
输出：承诺r(多项式N*1)
*/
void LRCT_Com(poly *r, poly *A, poly *sk, size_t mLen, unsigned char *bMessage, size_t msglen);
void LRCT_nttCom(poly *r, poly *A, poly *sk, size_t mLen, unsigned char *bMessage, size_t msglen);

/*
函数声明：矩阵A (N*M)* 矩阵s（M*1）
输入：矩阵A, 矩阵s,矩阵行数mLen
输出：矩阵r = A*s (N*1)
*/
void LRCT_MatrixMulPoly(poly *r, poly *A, poly *s, size_t mLen);
/*
函数声明:常数 * 矩阵
输入：常数cof, 矩阵A, 矩阵行数（mLen）
输出：矩阵r
*/
void LRCT_ConstMulMatrix(poly *r, const poly *A, uint16_t cof, size_t mLen);
/*
函数声明：矩阵A (M行N列)* 多项式p（N行1列）
输入：多项式p,矩阵A，矩阵行数mLen
输出：多项式r(M行1列)
*/
void LRCT_PolyMultMatrix(poly *r, poly *p, poly *A,  size_t mLen);
/*
函数声明：矩阵加法（A+B）
输入：矩阵A,矩阵B,矩阵大小mLen
输出：矩阵R=（A+B）
**/
void LRCT_MatrixAddMatrix(poly *R, poly *A, poly *B, size_t mLen);
/*
函数声明：矩阵减法（A-B）
输入：矩阵A,矩阵B,矩阵大小mLen
输出：矩阵R =（A-B）
**/
void LRCT_MatrixSubMatrix(poly *R, poly *A, poly *B, size_t mLen);

///////////////////*
/*
函数声明：密钥提取函数
输入：密钥长度mLen
输出：多项式矩阵r(n*mLen)
*/
/////////////////
void LRCT_SampleKey(poly *r, size_t mLen);

void LRCT_MatrixShift(poly *desCK, poly* rCK, size_t mLen, int iNumber);

void LRCT_GetCK0(poly *CK0, poly * CK, size_t mLen, poly* CKi, int messBitLen);
#endif

