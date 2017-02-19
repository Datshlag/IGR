import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class MainFramePart1 extends JFrame {

	private static final long serialVersionUID = 1L;
	private JTextArea textArea = null;
	private JPanel southPanel = null;
	private JScrollPane centerScrollPane = null;
	private JButton button1, button2, button3 = null;
	
	public static void main(String argv[]) {

		MainFramePart1 topLevel = new MainFramePart1();
	}

	public MainFramePart1() {

		super();

		/* Comportement de la zone de texte */
		textArea = new JTextArea(20, 100);
		centerScrollPane = new JScrollPane(textArea);

		add(centerScrollPane, BorderLayout.CENTER);

		/* Comportement des boutons */
		southPanel = new JPanel();

		button1 = new JButton("Hello !");
		button2 = new JButton("Bonjour !");
		button3 = new JButton("Exit");

		button1.addActionListener(new HelloListener());
		button2.addActionListener(new BonjourListener());
		button3.addActionListener(new ExitListener());

		southPanel.add(button1);
		southPanel.add(button2);
		southPanel.add(button3);

		add(southPanel, BorderLayout.SOUTH);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("Main Frame");
		pack();
		setVisible(true);
	}


	class BonjourListener implements ActionListener {

		public void actionPerformed(ActionEvent e) {

			textArea.append("\nBonjour ! ");
		}
	}

	class HelloListener implements ActionListener {

		public void actionPerformed(ActionEvent e) {

			textArea.append("Hello ! ");
		}
	}

	class ExitListener implements ActionListener {

		public void actionPerformed(ActionEvent e) {

			System.exit(0);
		}
	}
}
