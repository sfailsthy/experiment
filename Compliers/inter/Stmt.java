package inter;

public class Stmt extends Node {
	public Stmt() {
	}
	
	public static Stmt Null=new Stmt();
	
	public void gen(int b,int a){
		//����ʱ�Ĳ�������俪ʼ���ı�ź�������һ��ָ��ı��
	}
	
	int after=0;//����������һ��ָ��ı��
	public static Stmt Enclosing=Stmt.Null;//����break���
	
}
