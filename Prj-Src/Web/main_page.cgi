i pg_header.inc
t <a class="cur">������Ϣ</a>
t <a href="/system.cgi">ϵͳ����</a>
t <a href="/aplication_set.cgi">Ӧ������</a>
t <a href="/network.cgi">IP����</a>
t </td><td valign="top">
t <style>
t #m_table{margin:20px 0px 0px 20px;}
t #m_table tr td{padding:5px 30px 5px 30px;}
t #m_table tr td{background:#eee;}
t #m_table tr td.desc{background:#ccc;}
t </style>
t <form name="cgi" action="main_page.cgi" >
t <table id="m_table" border="0" cellspacing="1">
c d 1 <tr><td nowrap class="desc">�豸��Ϣ</td><td nowrap>%s</td></tr>
c d 2 <tr><td nowrap class="desc">�豸�ص�</td><td nowrap>%s</td></tr>
c d a <tr><td nowrap class="desc">�豸�ͺ�</td><td nowrap>%s</td></tr>
c d 3 <tr><td nowrap class="desc">����汾��</td><td nowrap>%s</td></tr>
c d 4 <tr><td nowrap class="desc">MAC��ַ</td><td nowrap>%s-%s-%s-%s-%s-%s</td></tr>
c d 5 <tr><td nowrap class="desc">����IP</td><td nowrap>%d.%d.%d.%d</td></tr>
c d 6 <tr><td nowrap class="desc">������IP</td><td nowrap>%d.%d.%d.%d</td></tr>
c d 7 <tr><td nowrap class="desc">�������˿�</td><td nowrap>%s</td></tr>
c d 8 <tr><td nowrap class="desc">�������ѹ��</td><td nowrap>%.1f</td></tr>
c d 9 <tr><td nowrap class="desc">������ѹʵʱ���(F5ˢ��)</td><td nowrap>%d V</td></tr>
c d b <tr><td nowrap class="desc">�Ƿ�ѧϰ</td><td nowrap>%s</td></tr>
c d c <tr><td nowrap class="desc">ѧϰ����Чͨ��(˳��:�̻ƺ�)</td><td nowrap>%s</td></tr>
c d e <tr><td nowrap class="desc">ѧϰ��ͨ������(˳��:�̻ƺ�)</td><td nowrap>%d %d %d %d %d %d %d %d %d %d %d %d</td></tr>
c d f <tr><td nowrap class="desc">����ʱ��</td><td nowrap>%d s</td></tr>
c d d <tr><td nowrap class="desc">��ǰ�׶α�</td><td nowrap>%s%d%s%d%d%d%d%d%d%d%d ��%ds</td></tr>
t <tr><td nowrap>&nbsp;</td><td nowrap class="btns">
t <input type="submit" value="ˢ��" id="confirm"/></td></tr></table></form>
i pg_footer.inc
. End of script must be closed with period.