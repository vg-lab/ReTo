int main() 
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_SCISSORS_TEST);
	glScissors(posX - 5, posY - 5, posX + 5, HEIGHT - posY + 5);

	for(Object o: objects) 
	{
		glStencilFunc(GL_ALWAYS, o.id(), -1);
		o.renderWithPick();
	}

	// Read pixel
	GLint index;
	glReadPixels(posX, HEIGHT - posY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &indxx);
	if(index >= 0) 
	{
		std::cout << "Clickado objeto" << index << std::end;
	} 
	else 
	{
		std::cout << "No se ha clickado" << std::endl;
	}

	glDisable(GL_SCISSORS_TEST);
	glDisable(GL_STENCIL_TEST);

	return 0;
}