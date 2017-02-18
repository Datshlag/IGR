import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class MainFrameFinal extends JFrame {

	private static final long serialVersionUID = 1L;
	private JTextArea requestTextArea;
	private JTextArea responseTextArea;
	private Client client;

	public MainFrameFinal(Client client) {

		super();

		/* client */
		this.client = client;	

		/* Comportement des zones de texte */
		requestTextArea = new JTextArea(2, 50);
		responseTextArea = new JTextArea(20, 50);
		responseTextArea.setEditable(false);

		requestTextArea.addKeyListener(new KeyListener() {

		    @Override
		    public void keyTyped(KeyEvent e) {
		    }

		    @Override
		    public void keyReleased(KeyEvent e) {
		        if(e.getKeyCode() == KeyEvent.VK_ENTER){
		            sendRequest();
		        }
		    }

		    @Override
		    public void keyPressed(KeyEvent e) {
		    }
		});

		JScrollPane requestScrollPane = new JScrollPane(requestTextArea);
		JScrollPane responseScrollPane = new JScrollPane(responseTextArea);

		/* Labels */
		JLabel requestLabel = new JLabel("Request :");
		JLabel responseLabel = new JLabel("Answer :");

		/* Actions */
		SendRequestAction sendRequestAction = new SendRequestAction("Send");
		ExitAction exitAction = new ExitAction("Exit");

		/* Menu */
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Request Menu");

		JMenuItem sendRequestMenuItem = new JMenuItem(sendRequestAction);
		JMenuItem exitMenuItem = new JMenuItem(exitAction);

		menu.add(sendRequestMenuItem);
		//menu.add(exitMenuItem);

		menuBar.add(menu);
		setJMenuBar(menuBar);

		/* Barre d'outils */
		JToolBar toolBar = new JToolBar();

		toolBar.add(new JButton(sendRequestAction));
		toolBar.add(new JButton(exitAction));

		add(toolBar, BorderLayout.NORTH);

		/* Layout */
		JPanel textPanel = new JPanel();
		textPanel.setLayout(new BoxLayout(textPanel, BoxLayout.Y_AXIS));
		textPanel.add(requestLabel);
		textPanel.add(requestScrollPane, BorderLayout.NORTH);
		textPanel.add(responseLabel);
		textPanel.add(responseScrollPane, BorderLayout.SOUTH);
		add(textPanel, BorderLayout.CENTER);

		/* JFrame */
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("Remote Controller");
		pack();
		setResizable(false);
		setVisible(true);

	}

	public JTextArea getResponseTextArea() {

		return responseTextArea;
	}

	public void sendRequest() {

		String[] request = requestTextArea.getText().split("\n");
		String response = client.send(request[0]);
		requestTextArea.setText("");
		responseTextArea.append("Request : " + request[0] + "\nServer answered : " + response + "\n");
	}

	class SendRequestAction extends AbstractAction {

		public SendRequestAction(String s) {

			super(s);
		}

		public void actionPerformed(ActionEvent e) {

			sendRequest();
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
