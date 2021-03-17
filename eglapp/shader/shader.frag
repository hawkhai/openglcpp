/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

precision lowp float;

varying vec2 vv2TexCoord;
varying vec3 vv3colour;

uniform sampler2D ourTexture;

void main() {
	gl_FragColor = mix(vec4(vv3colour, 1.0), 
            texture2D(ourTexture, vv2TexCoord), 0.5);
}
