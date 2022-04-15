import java.net.*;
import java.io.*;

/**
 * Класс служит для поддержки обработки серии запросов одного клиента
 */
public class CASession implements Runnable{

    /** Слово не найдено (не распознано) */
    private static String NOT_FOUND = "<answer><notfound></answer>";

    /** клиентский сокет для соединения клиентов с CompoundAnalyzer */
    private Socket clientSocket;

    /** поток для передачи запросов на RMU-сервер */
    private OutputStream rmuOutputStream;

    /** reader для получения ответов от RMU-сервера */
    private BufferedReader rmuReader;

    /** reader для получения запросов от клиентов */
    private BufferedReader clientReader;

    /** поток для передачи ответов клиентам */
    OutputStream clientOutputStream;

    /** код окончания запроса */
    private final static byte[] END_REQUEST = {10,10};

    /** код окончания ответа */
    private final static byte[] END_RESPONSE = {10,13,10,10};

    /** Writer для вывода на консоль русскоязычных сообщений */
    private OutputStreamWriter out;

    /** конструктор */
    public CASession(Socket clientSocket, BufferedReader in, OutputStream out) {
        this.clientSocket = clientSocket;
        rmuReader = in;
        rmuOutputStream = out;
        try {
            this.out = new OutputStreamWriter(System.out, "CP866");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
    }

    /** Читает и возвращает запрос клиента */
    private String readRequest() throws IOException {
        String request = clientReader.readLine();
        clientReader.read();
        return request;
    }

    /** Читает и возвращает ответ RMU-сервера */
    private String readResponse() throws IOException {
        String response = rmuReader.readLine();
        rmuReader.read();
        rmuReader.read();
        rmuReader.read();
        return response;
    }

    /** Отсылает заданную строку как запрос RMU-серверу */
    private void sendRequest(String request) throws IOException {
        rmuOutputStream.write(request.getBytes());
        rmuOutputStream.write(END_REQUEST);
    }

    /** Отсылает заданную строку как ответ клиенту */
    private void sendResponse(String response) throws IOException {
        clientOutputStream.write(response.getBytes());
        clientOutputStream.write(END_RESPONSE);
    }

    /** Основная функция обработки запроса клиента */
    private String processRequest(String requestString) throws IOException {
        String response = null;
        Word requestWord = new Word(requestString);
        if (!CompoundAnalyzer.findPrefix(requestWord)) {
	        out.write("Слово <" + requestWord + "> не распознано\n");
	        out.flush();
            return NOT_FOUND;
        }
        out.write("Исходное слово=<" + requestWord.whole + ">\n");
        out.write("Префикс=<" + requestWord.prefix + ">\n");
        out.write("Суффикс=<" + requestWord.suffix + ">\n");
        out.flush();
        sendRequest(CompoundAnalyzer.composeXMLRequest(requestWord.suffix));
        response = readResponse();
        out.write("Результат анализа суффикса=<" + response + ">\n");
        out.flush();
        if (!response.equals(NOT_FOUND)) {
            return CompoundAnalyzer.insertPrefix(response, requestWord.suffix, requestWord.prefix);
        }
        String recursiveResponse = processRequest(requestWord.suffix);
        if (!recursiveResponse.equals(NOT_FOUND)) {
            return CompoundAnalyzer.insertPrefix(recursiveResponse, requestWord.suffix, requestWord.prefix);
        }
        return NOT_FOUND;
    }

    /** В цикле обрабатывает последовательные запросы одного клиента */
    public void run() {
        System.out.println("Start session");

        try {
            // создать для клиента входной поток
            clientReader = new BufferedReader(
                new InputStreamReader(clientSocket.getInputStream(), "Cp1251")
            );

            // создать для клиента выходной поток
            clientOutputStream = clientSocket.getOutputStream();

            while(true) {
                // считать клиентский запрос
                String clientRequestString = readRequest();

                // обработать этот запрос
                String response = processRequest(CompoundAnalyzer.extractQuotedString(clientRequestString));

                // выдать клиенту результат обработки
                sendResponse(response);
            }

        } catch (SocketException ex) {
            System.out.println("Client connection closed");
        } catch (IOException e) {
            System.out.println("Exception in CASession");
            e.printStackTrace();
        } finally {
            try {
                clientReader.close();
                clientOutputStream.close();
                clientSocket.close();
            } catch (Exception ex) {
            }
        }
        System.out.println("Finish session");
    }
}
