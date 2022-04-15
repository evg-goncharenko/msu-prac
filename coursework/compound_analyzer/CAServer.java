import java.net.*;
import java.util.Properties;
import java.io.*;

/**
 * Сервер, который подсоединяется к RMU-серверу
 * и может обрабатывать запросы от нескольких клиентов.
 * Для каждого клиента создается собственный экземпляр CASession
 */

public class CAServer {

    /** Название файла свойств */
    private static String PROPERTIES_FILE = "CAServer.properties";

    /** сокет, через который CompoundAnalyzer соединяется с RMU-сервером */
    private Socket rmuSocket;

    /** порт для соединения с RMU-сервером */
    private static String RMU_PORT = "RMU_PORT";

    /** хост, на котором установлен RMU-сервер */
    private static String RMU_HOST = "RMU_HOST";

    /** поток для передачи запросов на RMU-сервер */
    private OutputStream rmuOutputStream;

    /** поток для получения ответов от RMU-сервера */
    private BufferedReader rmuReader;

    /** порт, который слушает CompoundAnalyzer */
    private static String CA_PORT = "CA_PORT";

    /** серверный сокет для соединения клиентов с CompoundAnalyzer */
    private ServerSocket caSocket;

    /** Конструктор */
    public CAServer() {
        try {
            // Загрузить информацию о сетевой конфигурации из внешнего файла
            InputStream in = new FileInputStream(PROPERTIES_FILE);
            Properties serverProperties = new Properties();
            serverProperties.load(in);
            in.close();

            // Инициализировать сервер
            caSocket = new ServerSocket(
                Integer.parseInt(serverProperties.getProperty(CA_PORT))
            );
            rmuSocket = new Socket(
                serverProperties.getProperty(RMU_HOST),
                Integer.parseInt(serverProperties.getProperty(RMU_PORT))
            );

            rmuOutputStream = rmuSocket.getOutputStream();
            rmuReader = new BufferedReader(
                new InputStreamReader(rmuSocket.getInputStream(), "Cp1251")
            );

        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    /** Основной цикл работы сервера */
    public void run() {
        System.out.println("Start server");
        try {
            while(true) {
                // клиент подключается к CompoundAnalyzer
                Socket clientSocket = caSocket.accept();
                                
                // создать для клиента новую сессию
                CASession session = new CASession(clientSocket, rmuReader, rmuOutputStream);

                // запустить сессию
                new Thread(session).start();
            }
        } catch (IOException e) {
            System.out.println("Exception in CAServer");
            e.printStackTrace();
        }
        System.out.println("Finish server");
    }
    
    /** Метод main() выполняется при запуске сервера */
    public static void main(String[] args) throws IOException {
       CAServer server = new CAServer();
       server.run();
    }
}
