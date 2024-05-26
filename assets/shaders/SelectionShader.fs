#version 330 core

flat in int v_ObjectID;

out int FragColor;

void main()
{
    FragColor = v_ObjectID;
}