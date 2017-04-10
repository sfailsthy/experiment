package symbols;

import lexer.*;

public class Array extends Type {
	public Type of;// �����Ԫ������
	public int size = 1;// Ԫ�ظ���

	public Array(int sz, Type p) {
		super("[]", Tag.INDEX, sz * p.width);
		size = sz;
		of = p;
	}

	public String toString() {
		return "[" + size + "]" + of.toString();
	}
}
