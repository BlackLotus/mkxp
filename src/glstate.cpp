/*
** glstate.cpp
**
** This file is part of mkxp.
**
** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
**
** mkxp is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** mkxp is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with mkxp.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "glstate.h"
#include "shader.h"
#include "etc.h"
#include "gl-fun.h"

#include <SDL_rect.h>

void GLClearColor::apply(const Vec4 &value)
{
	gl.ClearColor(value.x, value.y, value.z, value.w);
}

void GLScissorBox::apply(const IntRect &value)
{
	gl.Scissor(value.x, value.y, value.w, value.h);
}

void GLScissorBox::setIntersect(const IntRect &value)
{
	IntRect &current = get();

	SDL_Rect r1 = { current.x, current.y, current.w, current.h };
	SDL_Rect r2 = { value.x,   value.y,   value.w,   value.h };

	SDL_Rect result;
	if (!SDL_IntersectRect(&r1, &r2, &result))
		result.w = result.h = 0;

	set(IntRect(result.x, result.y, result.w, result.h));
}

void GLScissorTest::apply(const bool &value)
{
	value ? gl.Enable(GL_SCISSOR_TEST) : gl.Disable(GL_SCISSOR_TEST);
}

void GLBlendMode::apply(const BlendType &value)
{
	switch (value)
	{
	case BlendNone :
		gl.BlendEquation(GL_FUNC_ADD);
		gl.BlendFunc(GL_ONE, GL_ZERO);
		break;

	case BlendNormal :
		gl.BlendEquation(GL_FUNC_ADD);
		gl.BlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
		                     GL_ONE,       GL_ONE_MINUS_SRC_ALPHA);
		break;

	case BlendAddition :
		gl.BlendEquation(GL_FUNC_ADD);
		gl.BlendFuncSeparate(GL_SRC_ALPHA, GL_ONE,
		                     GL_ONE,       GL_ONE);
		break;

	case BlendSubstraction :
		// FIXME Alpha calculation is untested
		gl.BlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		gl.BlendFuncSeparate(GL_SRC_ALPHA, GL_ONE,
		                     GL_ONE,       GL_ONE);
		break;
	}
}

void GLViewport::apply(const IntRect &value)
{
	gl.Viewport(value.x, value.y, value.w, value.h);
}

void GLProgram::apply(const unsigned int &value)
{
	gl.UseProgram(value);
}

GLState::Caps::Caps()
{
	gl.GetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
}

GLState::GLState()
{
	gl.Enable(GL_BLEND);
	gl.Disable(GL_DEPTH_TEST);

	clearColor.init(Vec4(0, 0, 0, 1));
	blendMode.init(BlendNormal);
	scissorTest.init(false);
	scissorBox.init(IntRect(0, 0, 640, 480));
	program.init(0);
}
