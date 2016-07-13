;------------------------------------------------------------------------------
; Vertex shader for fog
;------------------------------------------------------------------------------
xvs.1.0
#pragma screenspace

;------------------------------------------------------------------------------
; Constants specified by the app
;    c0      = ( 0, 0, 0, 0)
;    c20-c23 = matWorld x matView
;    c30-c33 = matWorld x matView x matProj
;    c40     = light position
;    c41     = light diffuse color
;    c42     = light ambient color
;    c44     = fog values
;
; Vertex components (as specified in the vertex DECL)
;    v0    = Position
;    v2    = Normal
;    v5    = Texcoords
;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
; Vertex transformation
;------------------------------------------------------------------------------

; Transform vertex position
m4x4 oPos, v0, c30


;------------------------------------------------------------------------------
; Lighting calculation
;------------------------------------------------------------------------------

; Add point light
add r9, c40, -v0
dp3 r9.w, r9, r9
rsq r9.w, r9.w
dp3 r8.x, v2, r9
max r8.x, r8.x, c0.x
mul r8.x, r8.x, r9.w
mul r10, r8.x, c41
add oD0, r10, c42


;------------------------------------------------------------------------------
; Texture coordinates
;------------------------------------------------------------------------------

; Copy tex coords
mov oT0.xy, v5


;------------------------------------------------------------------------------
; Fog
;
; NOTE: the instructions below perform linear fog, as an example. This depends
; on the D3DRS_FOGTABLEMODE being set by the app to D3DFOG_NONE. Setting the
; renderstate this way gives the shader complete control over the oFog value.
;
; Alternatively, you can set D3DRS_FOGTABLEMODE to D3DFOG_LINEAR or D3DFOG_EXP
; which will cause the hardware to do the table lookup after the shader. In
; that case, your app will still need to set D3DRS_FOGSTART/END/DENSITY and
; output view space coordinates in oFog, as in the following:
;    ; Transform vertex to view space
;    dp4 r0.x, v0, c20
;    dp4 r0.y, v0, c21
;    dp4 r0.z, v0, c22
;
;    ; Use distance from vertex to eye as fog factor
;    dp3 r0.w, r0, r0
;    rsq r1.w, r0.w
;    mul oFog.x, r0.w, r1.w
;------------------------------------------------------------------------------

; Transform vertex to view space
dp4 r0.z, v0, c22

; Perform linear fog equation
mad oFog.x, r0.z, c44.x, c44.y


;------------------------------------------------------------------------------
; Final stuff
;------------------------------------------------------------------------------

; Screenspace instructions are interwoven to prevent a stall
mul oPos.xyz, r12, c-38
+rcc r1.x, r12.w

mad oPos.xyz, r12, r1.x, c-37
