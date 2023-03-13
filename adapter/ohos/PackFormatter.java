package ohos;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Formatter;
import java.util.logging.LogRecord;

public class PackFormatter extends Formatter {
    private static final int LOG_MAX_LIMIT = 1000;
    private static final DateFormat df = new SimpleDateFormat("yyyy/MM/dd hh:mm:ss.SSS");
    @Override
    public String format(LogRecord record) {
        StringBuilder builder = new StringBuilder(LOG_MAX_LIMIT);
        builder.append(df.format(new Date(record.getMillis()))).append(" - " );
        builder.append(formatMessage(record));
        builder.append("\n");
        return builder.toString();
    }
}