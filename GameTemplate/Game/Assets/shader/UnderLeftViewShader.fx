/*!
 * @brief	シンプルなモデルシェーダー。
 */

//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標。
    float2 uv : TEXCOORD0; //UV座標。
    float3 normal : NORMAL; //法線ベクトル
};
//ピクセルシェーダーへの入力。
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float2 uv : TEXCOORD0; //uv座標。
    float3 normal : NORMAL; //法線ベクトル
 };

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
Texture2D<float4> g_normalMap : register(t1); //ノーマルマップ
sampler g_sampler : register(s0); //サンプラステート。
////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;
    
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(mWorld, vsIn.normal);
    return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);

	//ディレクションライトの計算
    //法線とディレクションライトの向きの内積を求める
    //float t = dot(psIn.normal, float3(1.0f, 1.0f, 0.0f) * -1.0f);
    //t = max(0.0f, t);
    //finalColor.xyz *= float3(0.5f, 0.5f, 0.5f) * t;

    return finalColor;
}
