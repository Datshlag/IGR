import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class RemoteControl extends JFrame {

	private static final long serialVersionUID = 1L;
	private JTextArea requestTextArea;
	private JTextArea responseTextArea;
	private RemoteClient client;
    static final String DEFAULT_HOST = "localhost";
    static final int DEFAULT_PORT = 3331;

    /**
     * main function where we create the remote controle and the client.
     */
	public static void main(String argv[]) {

        String host = DEFAULT_HOST;
        int port = DEFAULT_PORT;
        if (argv.length >= 1) host = argv[0];
        if (argv.length >= 2) port = Integer.parseInt(argv[1]);
        
        RemoteClient remoteClient = null;
        
        try {

          remoteClient = new RemoteClient(host, port);
        }
        catch (Exception e) {

          System.err.println("Client: Couldn't connect to "+ host + ":" + port);
          System.exit(1);
        }
        
        System.out.println("Client connected to " + host + ":" + port);

        RemoteControl remoteControl = new RemoteControl(remoteClient);
    }

	public RemoteControl(RemoteClient client) {

		super();

		///The client
		this.client = client;	

		///Behaviour of textAreas. requestText Area is where the user writes the request
		///reponseTextArea is non editable : this is where we write answers from the server.
		requestTextArea = new JTextArea(2, 50);
		responseTextArea = new JTextArea(20, 50);
		responseTextArea.setEditable(false);

		///Here we add a listener to the requestArea so tha pressing enter will send the message.
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

		///Creating labels.
		JLabel requestLabel = new JLabel("Request :");
		JLabel responseLabel = new JLabel("Answer :");

		///Creating actions.
		SendRequestAction sendRequestAction = new SendRequestAction("Send");
		ExitAction exitAction = new ExitAction("Exit");

		///Creating the menu.
		JMenuBar menuBar = new JMenuBar();
		JMenu menu = new JMenu("Menu");

		JMenuItem sendRequestMenuItem = new JMenuItem(sendRequestAction);
		JMenuItem exitMenuItem = new JMenuItem(exitAction);

		menu.add(sendRequestMenuItem);
		menu.add(exitMenuItem);

		menuBar.add(menu);
		setJMenuBar(menuBar);

		///Creating the toolbar.
		JToolBar toolBar = new JToolBar();

		toolBar.add(new JButton(sendRequestAction));
		toolBar.add(new JButton(exitAction));

		add(toolBar, BorderLayout.NORTH);

		///Putting the layout together.
		JPanel textPanel = new JPanel();
		textPanel.setLayout(new BoxLayout(textPanel, BoxLayout.Y_AXIS));
		textPanel.add(requestLabel);
		textPanel.add(requestScrollPane, BorderLayout.NORTH);
		textPanel.add(responseLabel);
		textPanel.add(responseScrollPane, BorderLayout.SOUTH);
		add(textPanel, BorderLayout.CENTER);

		///Behaviour of the JFrame.
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("Remote Controller");
		pack();
		setResizable(false);
		setVisible(true);

	}

	/**
	 * Sends the request in the requestTextArea and displays the answer from the server in the responseTextArea.
	 */
	public void sendRequest() {

		String[] request = requestTextArea.getText().split("\n");
		String response = client.send(request[0]);
		requestTextArea.setText("");
		responseTextArea.append("Request : " + request[0] + "\nServer answered : " + response + "\n");
	}

	/**
	 * AbstractAction that we put in the menu and the tool bar, which is used to send the request.
	 */
	class SendRequestAction extends AbstractAction {

		public SendRequestAction(String s) {

			super(s);
		}

		public void actionPerformed(ActionEvent e) {

			sendRequest();
		}
	}

	/**
	 * AbstractAction we put in the manue and the tool bar, which is used to exit the app.
	 */
	class ExitAction extends AbstractAction {

		public ExitAction(String s) {

			super(s);
		}

		public void actionPerformed(ActionEvent e) {

			System.exit(0);
		}
	}
}
