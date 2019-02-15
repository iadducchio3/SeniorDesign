import smtplib
carriers = {
  'att':    '@mms.att.net',
  'tmobile':' @tmomail.net',
  'verizon':  '@vtext.com',
  'sprint':   '@page.nextel.com'
}

def send_message(message):
  to_number = '937-239-7094{}'.format(carriers['att'])
  auth = ('iadducchio3@gmail.com', '1224ia$$GOOGLE')

  server = smtplib.SMTP( "smtp.gmail.com", 587 )
  server.starttls()
  server.login(auth[0], auth[1])

  server.sendmail( auth[0], to_number, message)