import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class MainFramePart2 extends JFrame {

	private static final long serialVersionUID = 1L;
	private JTextArea textArea;
	
	public static void main(String argv[]) {

		MainFramePart2 topLevel = new MainFramePart2();
	}

	public MainFramePart2() {

		super();

		/* Comportement de la zone de texte */
		textArea = new JTextArea(20, 100);
		JScrollPane centerScrollPane = new JScrollPane(textArea);
		add(centerScrollPane, BorderLayout.CENTER);

		/* Comportement des actions */
		BonjourAction bAction = new BonjourAction("Bonjour !");
		HelloAction hAction = new HelloAction("Hello !");
		ExitAction eAction = new ExitAction("Exit");

		/* Comportement des menus */
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Simple Menu");

		JMenuItem bMenuItem = new JMenuItem(bAction);
		JMenuItem hMenuItem = new JMenuItem(hAction);
		JMenuItem eMenuItem = new JMenuItem(eAction);

		menu.add(bMenuItem);
		menu.add(hMenuItem);
		menu.add(eMenuItem);

		menuBar.add(menu);
		setJMenuBar(menuBar);

		/* Comportement barre d'outils */
		JToolBar toolBar = new JToolBar();

		toolBar.add(new JButton(bAction));
		toolBar.add(new JButton(hAction));
		toolBar.add(new JButton(eAction));

		add(toolBar, BorderLayout.NORTH);


		/* Comportement de la MainFrame */
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("Main Frame Part 2");
		pack();
		setVisible(true);
	}


	class BonjourAction extends AbstractAction {

		public BonjourAction(String s) {

			super(s);
		}

		public void actionPerformed(ActionEvent e) {

			textArea.append("\nBonjour ! ");
		}
	}

	class HelloAction extends AbstractAction {

		public HelloAction(String s) {

			super(s);
		}

		public void actionPerformed(ActionEvent e) {

			textArea.append("Hello ! ");
		}
	}

	class ExitAction extends AbstractAction {

		public ExitAction(String s) {

			super(s);
		}

		public void actionPerformed(ActionEvent e) {

			System.exit(0);
		}
	}
}
