file = open("BuildNumber.txt", 'r')
buildNumber = int(file.readline())
buildNumber += 1
file.close()
file = open("BuildNumber.txt", 'w')
file.write(str(buildNumber))
file.close()
